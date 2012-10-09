file (GLOB TCP_CLIENT_SOURCES *.cxx)

add_executable (tcp_client ${TCP_CLIENT_SOURCES})

target_link_libraries (
    tcp_client
    utils
    service
    io
    pthread
    ${Boost_LIBRARIES})
