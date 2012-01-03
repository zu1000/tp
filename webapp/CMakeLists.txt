cmake_minimum_required (VERSION 2.6)

project (WebClient)

include_directories("$ENV{WT_ROOT}/src")

add_executable(WebClient webclient.cxx)
