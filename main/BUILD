# 告诉Bazel项目的不同部分如何构建。工作空间中包含BUILD文件的目录称为包
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

# 构建一个 cc_library 库，cc_library 内置规则
cc_library(
    # 名称
    name = "hello-greet",
    # 依赖的源码
    srcs = ["hello-greet.cc"],
    # 头文件
    hdrs = ["include/hello-greet.h"],
)

# 定义个名为 hello-world 的目标，使用内置 cc_binary 规则，构建一个可执行文件
cc_binary(
    # 名称
    name = "hello-world",
    # 依赖的源码
    srcs = [
        "main.cc",
    ],
    # 依赖库，当前包下
    deps = [":hello-greet"],
)
