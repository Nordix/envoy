#pragma once

#include <signal.h>

#include <atomic>
#include <string>
#include <vector>

namespace Envoy {

class Restarter {
public:
  Restarter(const std::string envoy_binary, const std::vector<std::string>& watched_files,
            const std::vector<std::string>& envoy_args);
  void run();

private:
  void forkAndExecute();
  static void sigChldHandler(int sig, siginfo_t* info, void* context);

  int restart_epoch_;                      // hot-restart epoch counter
  std::string envoy_binary_;               // name of the command to execute
  std::vector<std::string> envoy_args_;    // arguments to pass to envoy
  std::vector<std::string> watched_files_; // list of files to watch with inotify

  static std::atomic<uint32_t> num_childs_; // number of child processes running currently
};

} // namespace Envoy
