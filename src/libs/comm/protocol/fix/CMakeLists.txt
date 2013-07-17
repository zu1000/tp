add_subdirectory(unittest)

file (GLOB FIX_PROTO_SOURCE *.cxx)

add_library (protocol_fix ${FIX_PROTO_SOURCE})
