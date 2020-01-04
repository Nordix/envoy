licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_binary(
    name = "restarter",
    srcs = ["main.cc"],
    deps = [
        ":restarter_lib",
    ],
)

envoy_cc_library(
    name = "restarter_lib",
    srcs = ["restarter.cc"],
    hdrs = ["restarter.h"],
    external_deps = ["abseil_base"],
    deps = [
        "//source/common/common:assert_lib",
    ],
)
