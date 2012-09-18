file (GLOB DB_SOURCE *.cxx)

add_library (database ${DB_SOURCE})

add_subdirectory (unittest)
