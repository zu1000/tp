file (GLOB GATEWAY_SOURCE *.cxx)

if (NOT ${GATEWAY_SOURCE} STREQUAL "")
    add_library (gateway ${GATEWAY_SOURCE})
endif (NOT ${GATEWAY_SOURCE} STREQUAL "")

add_subdirectory(unittest)
