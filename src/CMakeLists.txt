cmake_minimum_required(VERSION 3.18)

project(list)

set(SOURCE_LIB list.c)

add_library(list STATIC ${SOURCE_LIB})