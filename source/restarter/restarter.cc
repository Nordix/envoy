#include "restarter/restarter.h"

#include <limits.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <iostream>

#include "common/common/assert.h"
#include "common/common/fmt.h"

#include "absl/strings/str_join.h"

namespace Envoy {

std::atomic<uint32_t> Restarter::num_childs_{0};

Restarter::Restarter(const std::string envoy_binary, const std::vector<std::string>& watched_files,
                     const std::vector<std::string>& envoy_args)
    : restart_epoch_(0), envoy_binary_(envoy_binary), envoy_args_(envoy_args),
      watched_files_(watched_files) {}

void Restarter::run() {
  // install signal handler for catching SIGCHLD
  struct sigaction saction {};
  saction.sa_sigaction = &sigChldHandler;
  saction.sa_flags = SA_SIGINFO | SA_RESTART | SA_NOCLDSTOP;
  if (sigaction(SIGCHLD, &saction, 0) < 0) {
    PANIC("Restarter: sigaction() failed");
  }

  // create inotify watches
  int inotify_fd = inotify_init();
  if (inotify_fd == -1) {
    PANIC("Restarter: inotify_init() failed");
  }

  for (const auto& f : watched_files_) {
    if (inotify_add_watch(inotify_fd, f.c_str(), IN_MODIFY | IN_MOVED_TO) == -1) {
      PANIC(
          fmt::format("Restarter: error while adding watch for '{}': {}", f, std::strerror(errno)));
    }
  }

  std::cout << "Restarter: watching following files: " << absl::StrJoin(watched_files_, ", ")
            << std::endl;

  // execute first instance of envoy
  forkAndExecute();

  // process inotify events
  while (true) {
    uint8_t buffer[sizeof(inotify_event) + NAME_MAX + 1];
    ssize_t rc = read(inotify_fd, &buffer, sizeof(buffer));
    if (rc == -1 && errno == EAGAIN) {
      continue;
    }
    std::cout << "Restarter: watched files were updated" << std::endl;

    // don't care about event details - execute new instance for any subscribed event
    forkAndExecute();
  }
}

void Restarter::forkAndExecute() {
  pid_t pid = ::fork();
  if (pid == -1) {
    PANIC("Restarter: fork() failed");
  }

  // child process: execute new instance of envoy
  if (pid == 0) {
    // construct parameters for passing the epoch count
    std::string restart_epoch{"--restart-epoch"};
    std::string restart_epoch_count{std::to_string(restart_epoch_)};

    // construct null terminated array of parameters for exec()
    std::vector<char*> cargs;
    cargs.emplace_back(&envoy_binary_[0]);
    cargs.emplace_back(&restart_epoch[0]);
    cargs.emplace_back(&restart_epoch_count[0]);
    for (auto& s : envoy_args_) {
      cargs.emplace_back(&s[0]);
    }
    cargs.emplace_back(nullptr);

    std::cout << "Restarter: exec(): " << absl::StrJoin(cargs, " ") << std::endl;
    ::execvp(cargs[0], &cargs[0]);

    // execution never continues here unless exec failed
    std::cerr << fmt::format("execvp() failed: {}", std::strerror(errno)) << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // parent process: increment restart count
  restart_epoch_++;

  // keep track of number of spawned child processes
  num_childs_++;

  std::cout << fmt::format("Restarter: child pid={} num_childs={}", pid, num_childs_) << std::endl;

  return;
}

// NOTE: use only signal-safe functions in signal handler, signal-safety(7)
void Restarter::sigChldHandler(int /*sig*/, siginfo_t* si, void* /*context*/) {
  // keep track of number of spawned child processes
  num_childs_--;

  if ((si->si_code == CLD_EXITED) && (si->si_status == EXIT_SUCCESS)) {
    // child exited normally: call waitpid() to avoid zombies and continue normally
    int saved = errno;
    while (waitpid(-1, 0, WNOHANG) > 0) {
      // empty
    }
    errno = saved;

    // last child exited: exit the restarter in order to propagate error
    if (num_childs_ == 0) {
      char msg[] = "Restarter: last child exited - calling exit()\n";
      (void)::write(STDERR_FILENO, msg, ::strlen(msg));
      std::exit(EXIT_FAILURE);
    }

    char msg[] = "Restarter: child exited normally - ignoring\n";
    (void)::write(STDOUT_FILENO, msg, ::strlen(msg));

  } else {
    // child exited abnormally: exit the restarter in order to propagate error
    char msg[] = "Restarter: child exited abnormally - calling exit()\n";
    (void)::write(STDERR_FILENO, msg, ::strlen(msg));

    std::exit(si->si_status);
  }
}

} // namespace Envoy
