load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "rapidjson_lib",
    srcs = glob(
        # 前缀去除，原来是googletest-release-1.7.0/src/*.cc
        ["**/*.cc"],
    ),
    hdrs = glob([
        "**/*.h",
    ]),
    visibility = ["//visibility:public"],
)
