file (GLOB ECHO_SOURCES *.cxx)

add_executable (echo ${ECHO_SOURCES})

target_link_libraries (
    echo
    utils
    service
    io
    pthread
    ${Boost_LIBRARIES})
