file (GLOB UTILS_TEST_SOURCE *.cxx)

add_executable (test_utils ${UTILS_TEST_SOURCE})
target_link_libraries (test_utils
        utils
        xmllite
        pthread
        rt
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test (TestUtils test_utils)
