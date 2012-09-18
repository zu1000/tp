file (GLOB TICKER_DB_TEST_SOURCE *.cxx)

add_executable (test_ticker_database ${TICKER_DB_TEST_SOURCE})
target_link_libraries (test_ticker_database
        ticker_database
        utils
        database
        sqlite3
        ${Boost_LIBRARIES})

add_test (TestTickerDatabase test_ticker_database)
