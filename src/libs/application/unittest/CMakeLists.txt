file (GLOB APP_TEST_SOURCE *.cxx)

add_executable (test_application ${APP_TEST_SOURCE})
target_link_libraries (test_application
        application
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test (TestApplication test_application)
