#include <database/sql_update.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace database {
namespace test_sql_update {

    BOOST_AUTO_TEST_CASE(sql_update_simple)
    {
        sql_update update;

        std::string sql;

        BOOST_CHECK_EQUAL(false, update.sql_string(sql));
        BOOST_CHECK_EQUAL(true, sql.empty());

        update.set_table("test_update");

        BOOST_CHECK_EQUAL(false, update.sql_string(sql));
        BOOST_CHECK_EQUAL(true, sql.empty());

        update.set_update_key_value(update_key_value("key1", "value1"));

        std::string sql_expected =
            "update 'test_update' set key1='value1';";
        BOOST_CHECK_EQUAL(true, update.sql_string(sql));
        BOOST_CHECK_EQUAL(sql, sql_expected);

        update.set_update_key_value(update_key_value("key2", "value2"));
        sql_expected =
            "update 'test_update' set key1='value1',key2='value2';";
        BOOST_CHECK_EQUAL(true, update.sql_string(sql));
        BOOST_CHECK_EQUAL(sql, sql_expected);

        update.set_condition(update_condition("cond1", "cond1"));
        update.set_condition(update_condition("cond2", "cond2"));
        update.set_condition(update_condition("cond3", "cond3"), true, false);

        sql_expected =
            "update 'test_update' set key1='value1',key2='value2' "
            "where cond1='cond1' AND cond2='cond2' OR cond3='cond3';";
        BOOST_CHECK_EQUAL(true, update.sql_string(sql));
        BOOST_CHECK_EQUAL(sql, sql_expected);

        update.set_table("test_update1");
        update.set_update_key_value(update_key_value("key3", "value3"), false);
        update.set_condition(update_condition("cond4", "cond4"), false);

        sql_expected =
            "update 'test_update1' set key3='value3' where cond4='cond4';";
        BOOST_CHECK_EQUAL(true, update.sql_string(sql));
        BOOST_CHECK_EQUAL(sql, sql_expected);
    }

}}} // tp::database::test_sql_update
