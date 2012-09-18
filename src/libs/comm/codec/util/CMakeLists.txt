#add_subdirectory (unittest)

file (GLOB CODEC_UTIL_SOURCE *.cxx)

add_library (codec_util ${CODEC_UTIL_SOURCE})
