add_subdirectory(unittest)

file (GLOB INTERNAL_PROTO_SOURCE *.cxx)

add_library (protocol_internal ${INTERNAL_PROTO_SOURCE})
