file (GLOB INDICATOR_DB_SOURCES *.cxx)

add_executable (indicator_database ${INDICATOR_DB_SOURCES})

target_link_libraries (
    indicator_database
    application
    utils
    database
    service
    xmllite
    io
    binary
    codec_util
    protocol_internal
    sqlite3
    ${Boost_LIBRARIES})
