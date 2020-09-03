load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "list",
    srcs = glob(["**/list/code.c"]),
    hdrs = glob(["**/list/api.h", "**/list/data.h"]),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "list-light",
    srcs = glob(["**/list-light/code.c"]),
    hdrs = glob(["**/list-light/api.h", "**/list/data.h"]),
    visibility = ["//visibility:public"],
)