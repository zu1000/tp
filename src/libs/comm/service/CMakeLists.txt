add_subdirectory (unittest)
add_subdirectory (testserver)
add_subdirectory (testclient)

file (GLOB SERVICE_SOURCE *.cxx)

add_library (service ${SERVICE_SOURCE})
