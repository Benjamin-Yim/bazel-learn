load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

# 构建一个 cc_library 库，cc_library 内置规则
cc_library(
    # 名称
    name = "hello-greet",
    # 依赖的源码
    srcs = ["hello-greet.cc"],
    # 对外展示头文件
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
    args = [],
    copts = [
        # "-Iexternal/libevent2/WIN32-Code/nmake/",
        # "-Iexternal/libevent2/include",
        "-Iexternal/libevent2",
    ],
    # 依赖库，当前包下
    deps = [
        ":hello-greet",
        # 依赖其他报下的库
        "//main/lib:hello-time",
        "@libevent2//:libevent",
    ],
)
