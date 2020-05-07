load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],
    hdrs = ["include/hello-greet.h"],
)

cc_binary(
    name = "hello-world",
    srcs = [
        "main.cc",
    ],
    deps = [":hello-greet"],
)
