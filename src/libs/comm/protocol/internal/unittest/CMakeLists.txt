file (GLOB MESSAGE_INTERNAL_TEST_SOURCE *.cxx)

add_executable (test_message_internal ${MESSAGE_INTERNAL_TEST_SOURCE})
target_link_libraries (test_message_internal
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test (TestMessageInternal test_message_internal)
