file (GLOB PROTOCOL_INTERNAL_TEST_SOURCE *.cxx)

add_executable (test_protocol_internal ${PROTOCOL_INTERNAL_TEST_SOURCE})
target_link_libraries (test_protocol_internal
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test (TestProtocolInternal test_protocol_internal)
