file (GLOB IO_TEST_SOURCE *.cxx)

add_executable (test_io ${IO_TEST_SOURCE})
target_link_libraries (test_io
        io
        service
        pthread
        ${Boost_LIBRARIES})

add_test (TestIO test_io)
