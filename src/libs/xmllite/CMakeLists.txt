file (GLOB XMLLITE_SOURCE *.cxx)

add_library (xmllite ${XMLLITE_SOURCE})

add_subdirectory(unittest)
