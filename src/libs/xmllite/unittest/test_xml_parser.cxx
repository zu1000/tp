#include <xmllite/xml_parser.hpp>

#include <boost/test/unit_test.hpp>
#include <sstream>

namespace tp {
namespace xmllite {
namespace unittest {
namespace testxmlparser {

    struct xml_handler : public xmllite::xml_handler
    {
        void comment(
                const std::string& comment)
        {
            std::cout << "Comment: " << comment << std::endl;
        }

        void start_element(
                const std::string& key,
                const xml_attrs& attrs)
        {
            std::cout << "Tag Begin: " << key << std::endl;
            std::cout << "Attrs: " << attrs.size() << std::endl;

            xml_attrs::const_iterator it = attrs.begin();

            for (; it != attrs.end(); ++it)
            {
                std::cout << "Key: " << it->first
                    << " Value: " << it->second << std::endl;
            }
        }

        void end_element(
                const std::string& key,
                const std::string& data)
        {
            std::cout << "Tag End  : " << key << std::endl;
            std::cout << "Tag Data : " << data << std::endl;
        }
    };

    struct xml_parser_fixture : public xmllite::xml_parser
    {
        xml_parser_fixture()
            : xmllite::xml_parser(handler_)
        {}

        xml_handler handler_;
    };

    BOOST_FIXTURE_TEST_CASE(CommentTest1, xml_parser_fixture)
    {
        std::string input = "<!-- Comment --!>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(CommentTest2, xml_parser_fixture)
    {
        std::string input = "<!--!--!>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(CommentTest3, xml_parser_fixture)
    {
        std::string input = "<!-- <Tag1> Data <<<.>>> --!>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(Ignore, xml_parser_fixture)
    {
        std::string input = "<?xml ?>";
        std::istringstream iss(input);

        parsing(iss);
    }

    BOOST_FIXTURE_TEST_CASE(SingleTagTest1, xml_parser_fixture)
    {
        std::string input = "<Tag>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(SingleTagTest2, xml_parser_fixture)
    {
        std::string input = "<Tag></Tag>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(SingleTagTest3, xml_parser_fixture)
    {
        std::string input = "<Tag/>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(SingleTagTest4, xml_parser_fixture)
    {
        std::string input = "<Tag> Data </Tag>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(SingleTagTest5, xml_parser_fixture)
    {
        std::string input = "</Tag>";
        std::istringstream iss(input);

        BOOST_CHECK(!parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(SingleTagTest6, xml_parser_fixture)
    {
        std::string input = "<Tag>\n</Tag>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(SingleTagTest7, xml_parser_fixture)
    {
        std::string input = "<Tag>\nDataTag\n</Tag>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(MultiTagTest1, xml_parser_fixture)
    {
        std::string input = "<Tag1></Tag1><Tag2></Tag2>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(MultiTagTest2, xml_parser_fixture)
    {
        std::string input = "<Tag1>Data1</Tag1><Tag2>Data2</Tag2>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(MultiTagTest3, xml_parser_fixture)
    {
        std::string input = "<Tag1><Tag2></Tag2></Tag1>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(MultiTagTest4, xml_parser_fixture)
    {
        std::string input = "<Tag1><Tag2>Data2</Tag2></Tag1>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(MultiTagTest5, xml_parser_fixture)
    {
        std::string input = "<Tag1>Data1<Tag2>Data2</Tag2></Tag1>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(MultiTagTest6, xml_parser_fixture)
    {
        std::string input = "<Tag1><Tag2>Data2</Tag2>Data1</Tag1>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(MultiTagTest7, xml_parser_fixture)
    {
        std::string input = "<?xml?>\n<!--Comment--!><Tag1></Tag1>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(TagAttrTest1, xml_parser_fixture)
    {
        std::string input = "<Tag1 id=1>Data</Tag1>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(TagAttrTest2, xml_parser_fixture)
    {
        std::string input = "<Tag1 id = 1>Data</Tag1>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(TagAttrTest3, xml_parser_fixture)
    {
        std::string input = "<Tag1 id = 1 >Data</Tag1>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(TagAttrTest4, xml_parser_fixture)
    {
        std::string input = "<Tag1 id = 1 name=\"NAME\">Data</Tag1>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(TagAttrTest5, xml_parser_fixture)
    {
        std::string input = "<Tag1 id = 1 name=\"NAME\"/>";
        std::istringstream iss(input);

        BOOST_CHECK(parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(TagAttrTest6, xml_parser_fixture)
    {
        std::string input = "<Tag1 id = 1 name=NAME/>";
        std::istringstream iss(input);

        BOOST_CHECK(!parsing(iss));
    }

    BOOST_FIXTURE_TEST_CASE(XmlFileTest1, xml_parser_fixture)
    {
        std::string filename = XMLFILENAME;

        BOOST_CHECK(parsing(filename));
    }

}}}}//tp::xmllite::unittest::testxmlparser
