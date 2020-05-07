# 告诉Bazel项目的不同部分如何构建。工作空间中包含BUILD文件的目录称为包
load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "hello-time",
    srcs = ["hello-time.cc"],
    hdrs = ["hello-time.h"],
    # 可见性，只限于当前包下
    visibility = ["//main:__pkg__"],
)