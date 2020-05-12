load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# http_archive(
#     name = "libevent2",
#     build_file = "@//:libevent2.BUILD",
#     sha256 = "a65bac6202ea8c5609fd5c7e480e6d25de467ea1917c08290c521752f147283d",
#     strip_prefix = "libevent-2.1.11-stable",
#     url = "https://github.com/libevent/libevent/releases/download/release-2.1.11-stable/libevent-2.1.11-stable.tar.gz",
# )

http_archive(
    name = "rapidjson",
    build_file = "@//:rapidjson.BUILD",
    sha256 = "8e00c38829d6785a2dfb951bb87c6974fa07dfe488aa5b25deec4b8bc0f6a3ab",
    strip_prefix = "rapidjson-1.1.0",
    url = "https://github.com/Tencent/rapidjson/archive/v1.1.0.zip",
)

new_local_repository(
    name = "libevent2",
    build_file = "@//:libevent2.BUILD",
    path = "/usr/local/include",
)
