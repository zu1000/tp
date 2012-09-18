find_package(Wt REQUIRED)

find_package(Boost REQUIRED
    random
    date_time
    regex
    program_options
    signals
    system
    filesystem
    thread)

include_directories( ${Wt_INCLUDE_DIR} )

file (GLOB WEBAPP_SOURCES *.cxx)

add_executable (webapp ${WEBAPP_SOURCES})

target_link_libraries (
    webapp
    web
    ${Wt_HTTP_LIBRARY}
    ${Wt_EXT_LIBRARY}
    ${Wt_LIBRARY}
    ${Boost_LIBRARIES}
    pthread
    ssl)
