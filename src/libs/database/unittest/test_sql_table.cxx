#include <database/sql_table.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace database {

    BOOST_AUTO_TEST_CASE(sql_table_no_name)
    {
        sql_table table("");

        table.set_column("test_column");

        std::string sql;
        BOOST_CHECK_EQUAL(false, table.sql_string(sql));
    }

    BOOST_AUTO_TEST_CASE(sql_table_create)
    {
        sql_table table("test");

        std::string sql;
        BOOST_CHECK_EQUAL(false, table.sql_string(sql));

        table.set_column("test_column");
        std::string sql_expected =
            "create table 'test' (test_column varchar(40));";


        BOOST_CHECK_EQUAL(true, table.sql_string(sql));
        BOOST_CHECK_EQUAL(sql_expected, sql);

        table.set_column("test_column1");
        sql_expected =
            "create table 'test' "
            "(test_column varchar(40),test_column1 varchar(40));";

        BOOST_CHECK_EQUAL(true, table.sql_string(sql));
        BOOST_CHECK_EQUAL(sql_expected, sql);
    }

    BOOST_AUTO_TEST_CASE(sql_table_drop)
    {
        sql_table table("test", false);

        std::string sql;

        BOOST_CHECK_EQUAL(true, table.sql_string(sql));

        std::string sql_expected =
            "drop table 'test';";

        BOOST_CHECK_EQUAL(sql_expected, sql);
    }

}}//tp::database
