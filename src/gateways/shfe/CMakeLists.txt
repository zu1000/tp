find_path(SHFE_INCLUDE_DIR
    NAMES
    ThostFtdcMdApi.h
    PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/shfe_libs
    PATH_SUFFIXES
    shfe_libs
    include)

find_library(SHFE_LIBRARIES
    NAMES
    thostmduserapi
    PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/shfe_libs
    PATH_SUFFIXES
    shfe_libs
    lib64)

message (STATUS "Using ${CMAKE_CURRENT_SOURCE_DIR}")
message (STATUS "Using ${SHFE_INCLUDE_DIR}")
message (STATUS "Using ${SHFE_LIBRARIES}")

include_directories( ${SHFE_INCLUDE_DIR} )

file (GLOB SHFE_GW_SOURCES *.cxx)

add_executable (gw_shfe ${SHFE_GW_SOURCES})

target_link_libraries (
    gw_shfe
    application
    utils
    service
    io
    binary
    codec_util
    protocol_internal
    xmllite
    pthread
    ${SHFE_LIBRARIES}
    ${Boost_LIBRARIES})
