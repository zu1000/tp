file (GLOB SINA_GW_SOURCES *.cxx)

add_executable (gw_sina ${SINA_GW_SOURCES})

target_link_libraries (
    gw_sina
    ${Boost_LIBRARIES})
