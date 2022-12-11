load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "list",
    srcs = glob(["**/list/code.c"]),
    hdrs = glob(["**/list/api.h", "**/list/data.h"]),
    copts = ["-Isrc"],
    visibility = ["//visibility:public"],
)
