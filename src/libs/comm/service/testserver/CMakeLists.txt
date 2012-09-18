file (GLOB SERVICE_TESTSERVER_SOURCE *.cxx)

add_executable (testserver_service ${SERVICE_TESTSERVER_SOURCE})
target_link_libraries (testserver_service
        service
        binary
        codec_util
        protocol_internal
        io
        pthread
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY})
