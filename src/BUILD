load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "list",
    srcs = glob(["**/src/list/code.c"]),
    hdrs = glob(["**/src/list/data.h"]),
    copts = ["-Isrc"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "list-light",
    srcs = glob(["**/src/list-light/code.c"]),
    hdrs = glob(["**/src/list-light/data.h"]),
    copts = ["-Isrc"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "list-micro",
    srcs = glob(["**/src/list-micro/code.c"]),
    hdrs = glob(["**/src/list-micro/data.h"]),
    copts = ["-Isrc"],
    visibility = ["//visibility:public"],
)
