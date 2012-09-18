file (GLOB APP_SOURCE *.cxx)

add_library (application ${APP_SOURCE})

add_subdirectory (unittest)
