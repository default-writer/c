load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "list",
    srcs = glob(["**/list.c"]),
    hdrs = glob(["**/list-api.h", "**/list.h"]),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "list-light",
    srcs = glob(["**/list-light.c"]),
    hdrs = glob(["**/list-light-api.h", "**/list.h"]),
    visibility = ["//visibility:public"],
)