#include <database/sql_insert.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace database {

    BOOST_AUTO_TEST_CASE(insert_no_name)
    {
        sql_insert insert("");

        column_value value;

        BOOST_CHECK_EQUAL(false, insert.set_column_value(value));

        value.first  = "test_column";
        value.second = "test_value";

        BOOST_CHECK_EQUAL(true, insert.set_column_value(value));

        std::string sql;

        BOOST_CHECK_EQUAL(false, insert.sql_string(sql));
    }

    BOOST_AUTO_TEST_CASE(sql_insert_insert)
    {
        std::string sql;

        sql_insert insert("test");

        column_value value;

        value.first  = "test_column";
        value.second = "test_value";

        BOOST_CHECK_EQUAL(true, insert.set_column_value(value));
        BOOST_CHECK_EQUAL(true, insert.sql_string(sql));

        std::string sql_expected =
            "insert into 'test' (test_column) values ('test_value');";

        BOOST_CHECK_EQUAL(sql_expected, sql);

        value.first  = "test_column1";
        value.second = "test_value1";

        BOOST_CHECK_EQUAL(true, insert.set_column_value(value));
        BOOST_CHECK_EQUAL(true, insert.sql_string(sql));

        sql_expected =
            "insert into 'test' (test_column,test_column1) "
            "values ('test_value','test_value1');";
    }

}} // tp::database
