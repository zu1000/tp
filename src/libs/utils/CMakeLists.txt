file (GLOB UTILS_SOURCE *.cxx)

add_library (utils ${UTILS_SOURCE})

add_subdirectory (unittest)
