file (GLOB PROTOCOL_FIX_TEST_SOURCE *.cxx)

add_executable (test_protocol_fix ${PROTOCOL_FIX_TEST_SOURCE})
target_link_libraries (test_protocol_fix
        protocol_fix
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test (TestProtocolFix test_protocol_fix)
