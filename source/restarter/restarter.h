#pragma once

#include <signal.h>

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

  int restart_epoch_;
  std::string envoy_binary_;
  std::vector<std::string> watched_files_;
  std::vector<std::string> envoy_args_;
};

} // namespace Envoy
