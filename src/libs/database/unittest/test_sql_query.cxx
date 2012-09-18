#include <database/sql_query.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace database {
namespace test_sql_query {

    BOOST_AUTO_TEST_CASE(sql_query_simple)
    {
        sql_query query;

        std::string sql;

        BOOST_CHECK_EQUAL(false, query.sql_string(sql));
        BOOST_CHECK_EQUAL(true, sql.empty());

        query.set_table("test_table");
        query.set_return_column("test_column");

        std::string sql_expected =
            "select test_column from 'test_table';";

        BOOST_CHECK_EQUAL(true, query.sql_string(sql));
        BOOST_CHECK_EQUAL(sql_expected, sql);

        query.set_table("test_table1");
        query.set_return_column("test_column1");

        sql_expected =
            "select test_column,test_column1 from 'test_table','test_table1';";

        BOOST_CHECK_EQUAL(true, query.sql_string(sql));
        BOOST_CHECK_EQUAL(sql_expected, sql);

        query.set_table("test_table2", false);
        query.set_return_column("test_column2", false);
        sql_expected =
            "select test_column2 from 'test_table2';";

        BOOST_CHECK_EQUAL(true, query.sql_string(sql));
        BOOST_CHECK_EQUAL(sql_expected, sql);

        query.set_condition(query_condition("a", "A"));
        query.set_condition(query_condition("b", "B"));

        sql_expected =
            "select test_column2 from 'test_table2' where a='A' AND b='B';";

        BOOST_CHECK_EQUAL(true, query.sql_string(sql));
        BOOST_CHECK_EQUAL(sql_expected, sql);

        query.set_condition(query_condition("a", "A"), false);
        query.set_condition(query_condition("b", "B"), true, false);

        sql_expected =
            "select test_column2 from 'test_table2' where a='A' OR b='B';";

        BOOST_CHECK_EQUAL(true, query.sql_string(sql));
        BOOST_CHECK_EQUAL(sql_expected, sql);

    }

}}}// tp::database::test_sql_query
