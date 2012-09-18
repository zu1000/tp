file (GLOB BIN_TEST_SOURCE *.cxx)

add_executable (test_bin ${BIN_TEST_SOURCE})
target_link_libraries (test_bin
    binary
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(TestBin test_bin)
