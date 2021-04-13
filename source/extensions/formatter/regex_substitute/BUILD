load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_extension",
    "envoy_cc_library",
    "envoy_extension_package",
)

licenses(["notice"])  # Apache 2

envoy_extension_package()

# Access log formatter that is used to substitute strings in request headers before printing them into access log.
# Public docs: docs/root/TODO(tsaarni)

envoy_cc_library(
    name = "regex_substitute_lib",
    srcs = ["regex_substitute.cc"],
    hdrs = ["regex_substitute.h"],
    deps = [
        "//source/common/formatter:substitution_formatter_lib",
        "//source/common/protobuf:utility_lib",
        "@envoy_api//envoy/extensions/formatter/regex_substitute/v3:pkg_cc_proto",
    ],
)

envoy_cc_extension(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    category = "envoy.formatter",
    security_posture = "unknown",
    status = "alpha",
    deps = [
        "//include/envoy/registry",
        "//source/extensions/formatter/regex_substitute:regex_substitute_lib",
        "@envoy_api//envoy/extensions/formatter/regex_substitute/v3:pkg_cc_proto",
        "//source/common/protobuf:utility_lib",
    ],
)
