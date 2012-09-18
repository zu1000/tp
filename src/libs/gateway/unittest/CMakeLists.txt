file (GLOB GW_TEST_SOURCE *.cxx)

add_executable (test_gateway ${GW_TEST_SOURCE})
target_link_libraries (test_gateway
        gateway
        ${Boost_THREAD_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test (TestGateway test_gateway)
