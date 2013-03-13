set (LIB_TICKER_DB_SOURCES database.cxx)
set (BIN_TICKER_DB_SOURCES application.cxx main.cxx)

add_library    (ticker_database ${LIB_TICKER_DB_SOURCES})
add_executable (ticker_database_app ${BIN_TICKER_DB_SOURCES})

target_link_libraries (
    ticker_database_app
    ticker_database
    application
    utils
    database
    service
    io
    binary
    codec_util
    protocol_internal
    xmllite
    sqlite3
    rt
    ${Boost_LIBRARIES})

add_subdirectory(unittest)
