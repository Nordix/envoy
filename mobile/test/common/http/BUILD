load("@envoy//bazel:envoy_build_system.bzl", "envoy_cc_test", "envoy_mobile_package")

licenses(["notice"])  # Apache 2

envoy_mobile_package()

envoy_cc_test(
    name = "client_test",
    srcs = ["client_test.cc"],
    repository = "@envoy",
    deps = [
        "//library/common/http:client_lib",
        "//library/common/http:header_utility_lib",
        "//library/common/types:c_types_lib",
        "//test/common/mocks/common:common_mocks",
        "//test/common/mocks/event:event_mocks",
        "@envoy//source/common/http:context_lib",
        "@envoy//source/common/stats:isolated_store_lib",
        "@envoy//test/common/http:common_lib",
        "@envoy//test/mocks/buffer:buffer_mocks",
        "@envoy//test/mocks/event:event_mocks",
        "@envoy//test/mocks/http:api_listener_mocks",
        "@envoy//test/mocks/local_info:local_info_mocks",
        "@envoy//test/mocks/upstream:upstream_mocks",
    ],
)

envoy_cc_test(
    name = "header_utility_test",
    srcs = ["header_utility_test.cc"],
    repository = "@envoy",
    deps = [
        "//library/common/bridge:utility_lib",
        "//library/common/http:header_utility_lib",
        "//library/common/types:c_types_lib",
        "@envoy//source/common/buffer:buffer_lib",
        "@envoy//source/common/http:header_map_lib",
    ],
)
