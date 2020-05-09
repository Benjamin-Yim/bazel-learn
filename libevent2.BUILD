load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "main",
    srcs = glob(
        # 前缀去除，原来是googletest-release-1.7.0/src/*.cc
        ["src/*.cc"],
        # 排除此文件
        exclude = ["src/gtest-all.cc"],
    ),
    hdrs = glob([
        # 前缀去除
        "include/**/*.h",
        "src/*.h",
    ]),
    copts = [
        # 前缀去除，原来是external/gtest/googletest-release-1.7.0/include
        "-Iexternal/gtest/include",
    ],
    # 链接到pthread
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)
