cmake_minimum_required(VERSION 3.4.3)
project(jit_exp)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "LLVM CMake PATH ${CMAKE_MODULE_PATH}")
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

# Now build our tools
add_executable(jit_exp jit_exp.cc)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(
    llvm_libs
    support
    core
    irreader
    mcjit
    x86codegen
    x86asmparser
    interpreter
    )

# Link against LLVM libraries
target_link_libraries(jit_exp ${llvm_libs})
