file (GLOB XMLLITE_TEST_SOURCE *.cxx)

add_definitions(
        -DXMLFILENAME="${CMAKE_SOURCE_DIR}/libs/xmllite/unittest/services_samples.XML" )

add_executable (test_xmllite ${XMLLITE_TEST_SOURCE})
target_link_libraries (test_xmllite
        xmllite
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test (TestXMLLite test_xmllite)
