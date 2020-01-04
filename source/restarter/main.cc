#include <getopt.h>

#include <iostream>
#include <string>
#include <vector>

#include "restarter/restarter.h"

// NOLINT(namespace-envoy)

void usage() {
  std::cerr << "Usage: restarter [--command FILE] [--watch FILE]... [-- envoy arguments]"
            << std::endl;
  std::exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
  // process command line arguments
  static struct option long_options[] = {
      {"command", required_argument, 0, 'c'},
      {"watch", required_argument, 0, 'w'},
      {0, 0, 0, 0},
  };
  int option_index = 0;
  std::vector<std::string> watched_files;
  std::string command = "envoy";

  while (true) {
    int c = getopt_long(argc, argv, "c:w:", long_options, &option_index);

    // no more arguments to be processed
    if (c == -1) {
      break;
    }

    switch (c) {
    case 'c':
      command = optarg;
      break;
    case 'w':
      watched_files.push_back(optarg);
      break;
    default:
      usage();
    }
  }

  // rest of the arguments to be passed directly to envoy
  std::vector<std::string> envoy_args(&argv[optind], &argv[argc]);

  Envoy::Restarter restarter(command, watched_files, envoy_args);
  restarter.run();

  return EXIT_SUCCESS;
}
