file (GLOB DB_TEST_SOURCE *.cxx)

add_executable (test_database ${DB_TEST_SOURCE})
target_link_libraries (test_database
        database
        sqlite3
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test (TestDatabase test_database)
