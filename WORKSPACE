# load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# http_archive(
#     name = "libevent2",
#     build_file = "@//:libevent2.BUILD",
#     sha256 = "a65bac6202ea8c5609fd5c7e480e6d25de467ea1917c08290c521752f147283d",
#     strip_prefix = "libevent-2.1.11-stable",
#     url = "https://github.com/libevent/libevent/releases/download/release-2.1.11-stable/libevent-2.1.11-stable.tar.gz",
# )
new_local_repository(
    name = "libevent2",
    build_file = "@//:libevent2.BUILD",
    path = "/usr/local/include",
)
