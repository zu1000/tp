file (GLOB SERVICE_TEST_SOURCE *.cxx)

add_executable (test_service ${SERVICE_TEST_SOURCE})
target_link_libraries (test_service
        service
        binary
        protocol_internal
        xmllite
        pthread
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY})

add_test (TestService test_service)
