add_subdirectory (unittest)

file (GLOB BINARY_SOURCE *.cxx)

add_library (binary ${BINARY_SOURCE})
