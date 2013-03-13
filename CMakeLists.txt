cmake_minimum_required (VERSION 2.6)

project (TrendPredictor)

enable_testing()

set (CMAKE_MODULE_PATH
     ${CMAKE_MODULE_PATH}
     ${PROJECT_SOURCE_DIR}
     ${PROJECT_SOURCE_DIR}/cmake)

set (TP_INCLUDE_DIR
     ${PROJECT_SOURCE_DIR}/src
     ${PROJECT_SOURCE_DIR}/src/libs)

include_directories ( ${TP_INCLUDE_DIR} )
#include_directories ( ${Wt_INCLUDE_DIR} )

find_package (Boost REQUIRED
        unit_test_framework
        thread
        system
        program_options)

message (STATUS "Using ${Boost_INCLUDE_DIRS}")
message (STATUS "Using ${Boost_LIBRARY_DIRS}")
message (STATUS "Using ${Boost_LIBRARIES}")

if (NOT Boost_USE_STATIC_LIBS)
    add_definitions(-DBOOST_TEST_DYN_LINK)
endif (NOT Boost_USE_STATIC_LIBS)

include_directories ( ${Boost_INCLUDE_DIRS} )

find_package (SQLite3 REQUIRED)

add_subdirectory(src)
