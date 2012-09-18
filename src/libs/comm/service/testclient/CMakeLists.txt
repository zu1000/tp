file (GLOB SERVICE_TESTCLIENT_SOURCE *.cxx)

add_executable (testclient_service ${SERVICE_TESTCLIENT_SOURCE})
target_link_libraries (testclient_service
        service
        binary
        codec_util
        protocol_internal
        io
        pthread
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY})
