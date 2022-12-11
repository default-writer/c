load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "main",
    srcs = glob(["**/tests/main.c"]),
    copts = ["-Isrc"],
    visibility = ["//visibility:public"],
)
