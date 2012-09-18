find_package(Wt REQUIRED)

include_directories( ${Wt_INCLUDE_DIR} )

file (GLOB WEB_SOURCE *.cxx)

add_library (web ${WEB_SOURCE})
