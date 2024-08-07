load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_extension",
    "envoy_extension_package",
    "envoy_proto_library",
)

licenses(["notice"])  # Apache 2

envoy_extension_package()

envoy_proto_library(
    name = "filter",
    srcs = ["filter.proto"],
)

envoy_cc_extension(
    name = "network_configuration_filter_lib",
    srcs = ["filter.cc"],
    hdrs = ["filter.h"],
    repository = "@envoy",
    deps = [
        ":filter_cc_proto",
        "//library/common/api:external_api_lib",
        "//library/common/http:header_utility_lib",
        "//library/common/network:connectivity_manager_lib",
        "//library/common/network:proxy_api_lib",
        "//library/common/network:proxy_resolver_interface_lib",
        "//library/common/network:proxy_settings_lib",
        "//library/common/stream_info:extra_stream_info_lib",
        "//library/common/types:c_types_lib",
        "@envoy//envoy/http:codes_interface",
        "@envoy//envoy/http:filter_interface",
        "@envoy//source/common/common:thread_lib",
        "@envoy//source/common/grpc:common_lib",
        "@envoy//source/common/grpc:status_lib",
        "@envoy//source/common/http:codes_lib",
        "@envoy//source/common/http:header_map_lib",
        "@envoy//source/common/http:headers_lib",
        "@envoy//source/common/http:utility_lib",
        "@envoy//source/common/network:filter_state_proxy_info_lib",
        "@envoy//source/extensions/filters/http/common:pass_through_filter_lib",
    ],
)

envoy_cc_extension(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    repository = "@envoy",
    deps = [
        ":network_configuration_filter_lib",
        "@envoy//source/extensions/filters/http/common:factory_base_lib",
    ],
)
