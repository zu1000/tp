file (GLOB IO_SOURCE *.cxx)
file (GLOB_RECURSE IO_IMPL_SOURCE impl/*.cxx)
add_library (io ${IO_SOURCE} ${IO_IMPL_SOURCE})

add_subdirectory(unittest)
