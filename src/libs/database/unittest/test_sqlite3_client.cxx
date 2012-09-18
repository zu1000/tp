#include <database/sqlite3_client.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace database {
namespace test_sqlite_client {

    BOOST_AUTO_TEST_CASE(create_drop_table)
    {
        sqlite3_client client("test_db");
        sql_table table("test_table");
        table.set_column("dummy1");

        BOOST_CHECK_EQUAL(true, client.table(table));

        // the second try should succeed as well, since table is already
        // created.
        BOOST_CHECK_EQUAL(true, client.table(table));

        // drop the table
        table.create(false);
        BOOST_CHECK_EQUAL(true, client.table(table));

    }

    BOOST_AUTO_TEST_CASE(insert_row)
    {
        sqlite3_client client("test_db");

        sql_table table("test_table");
        table.set_column("dummy1");
        table.set_column("dummy2");

        BOOST_CHECK_EQUAL(true, client.table(table));

        sql_insert insert("test_table");

        insert.set_column_value(column_value("dummy1", "data1"));
        insert.set_column_value(column_value("dummy2", "data2"));

        BOOST_CHECK_EQUAL(true, client.insert(insert));

        table.create(false);
        BOOST_CHECK_EQUAL(true, client.table(table));
    }

    BOOST_AUTO_TEST_CASE(update_row)
    {
        sqlite3_client client("test_db");

        sql_table table("test_table");
        table.set_column("dummy1");
        table.set_column("dummy2");

        BOOST_CHECK_EQUAL(true, client.table(table));

        sql_insert insert("test_table");

        insert.set_column_value(column_value("dummy1", "data1"));
        insert.set_column_value(column_value("dummy2", "data2"));

        BOOST_CHECK_EQUAL(true, client.insert(insert));

        sql_update update;

        update.set_table("test_table");

        update.set_update_key_value(update_key_value("dummy1", "date3"));

        BOOST_CHECK_EQUAL(true, client.update(update));

        table.create(false);
        BOOST_CHECK_EQUAL(true, client.table(table));
    }

    BOOST_AUTO_TEST_CASE(update_row_with_condition)
    {
        sqlite3_client client("test_db");

        sql_table table("test_table");
        table.set_column("dummy1");
        table.set_column("dummy2");

        BOOST_CHECK_EQUAL(true, client.table(table));

        sql_insert insert("test_table");

        insert.set_column_value(column_value("dummy1", "data1"));
        insert.set_column_value(column_value("dummy2", "data2"));

        BOOST_CHECK_EQUAL(true, client.insert(insert));

        sql_update update;

        update.set_table("test_table");

        update.set_update_key_value(update_key_value("dummy1", "date3"));
        update.set_condition(update_condition("dummy2", "data2"));

        BOOST_CHECK_EQUAL(true, client.update(update));

        update.set_update_key_value(update_key_value("dummy1", "date3"), false);
        update.set_condition(update_condition("dummy3", "data3"), false);

        BOOST_CHECK_EQUAL(false, client.update(update));

        table.create(false);
        BOOST_CHECK_EQUAL(true, client.table(table));
    }

    std::vector<query_result_row> rows; 
    static bool query_result_handler(const query_result_row& row)
    {
        rows.push_back(row);

        return true;
    }

    BOOST_AUTO_TEST_CASE(query_row)
    {
        sqlite3_client client("test_db");

        sql_table table("test_table");
        table.set_column("dummy1");
        table.set_column("dummy2");

        BOOST_CHECK_EQUAL(true, client.table(table));

        sql_insert insert("test_table");

        insert.set_column_value(column_value("dummy1", "data1"));
        insert.set_column_value(column_value("dummy2", "data2"));

        BOOST_CHECK_EQUAL(true, client.insert(insert));

        sql_query query;
        query.set_table("test_table");
        query.set_return_column("*");

        rows.clear();

        BOOST_CHECK_EQUAL(true, client.query(query, query_result_handler));

        BOOST_CHECK_EQUAL(1u, rows.size());

        const query_result_row& row = rows[0];

        BOOST_CHECK_EQUAL(2u, row.size());

        BOOST_CHECK_EQUAL(row[0].first , "dummy1");
        BOOST_CHECK_EQUAL(row[0].second, "data1");
        BOOST_CHECK_EQUAL(row[1].first , "dummy2");
        BOOST_CHECK_EQUAL(row[1].second, "data2");
        
        table.create(false);
        BOOST_CHECK_EQUAL(true, client.table(table));
    }

    BOOST_AUTO_TEST_CASE(query_row_failed)
    {
        sqlite3_client client("test_db");

        sql_table table("test_table");
        table.set_column("dummy1");
        table.set_column("dummy2");

        BOOST_CHECK_EQUAL(true, client.table(table));

        sql_insert insert("test_table");

        insert.set_column_value(column_value("dummy1", "data1"));
        insert.set_column_value(column_value("dummy2", "data2"));

        BOOST_CHECK_EQUAL(true, client.insert(insert));

        sql_query query;
        query.set_table("wrong_table");
        query.set_return_column("*");

        rows.clear();

        BOOST_CHECK_EQUAL(false, client.query(query, query_result_handler));

        BOOST_CHECK_EQUAL(0u, rows.size());

        table.create(false);
        BOOST_CHECK_EQUAL(true, client.table(table));
    }

    BOOST_AUTO_TEST_CASE(query_row_single_column)
    {
        sqlite3_client client("test_db");

        sql_table table("test_table");
        table.set_column("dummy1");
        table.set_column("dummy2");

        BOOST_CHECK_EQUAL(true, client.table(table));

        sql_insert insert("test_table");

        insert.set_column_value(column_value("dummy1", "data1"));
        insert.set_column_value(column_value("dummy2", "data2"));

        BOOST_CHECK_EQUAL(true, client.insert(insert));

        sql_query query;
        query.set_table("test_table");
        query.set_return_column("dummy1");

        rows.clear();

        BOOST_CHECK_EQUAL(true, client.query(query, query_result_handler));

        BOOST_CHECK_EQUAL(1u, rows.size());

        const query_result_row& row = rows[0];

        BOOST_CHECK_EQUAL(1u, row.size());

        BOOST_CHECK_EQUAL(row[0].first , "dummy1");
        BOOST_CHECK_EQUAL(row[0].second, "data1");

        table.create(false);
        BOOST_CHECK_EQUAL(true, client.table(table));
    }

    BOOST_AUTO_TEST_CASE(query_row_with_condition)
    {
        sqlite3_client client("test_db");

        sql_table table("test_table");
        table.set_column("dummy1");
        table.set_column("dummy2");

        BOOST_CHECK_EQUAL(true, client.table(table));

        sql_insert insert("test_table");

        insert.set_column_value(column_value("dummy1", "data1"));
        insert.set_column_value(column_value("dummy2", "data2"));

        BOOST_CHECK_EQUAL(true, client.insert(insert));

        insert.set_column_value(column_value("dummy1", "data3"), false);
        insert.set_column_value(column_value("dummy2", "data4"));

        BOOST_CHECK_EQUAL(true, client.insert(insert));

        sql_query query;
        query.set_table("test_table");
        query.set_return_column("dummy1");
        query.set_condition(query_condition("dummy2", "data4"));

        rows.clear();

        BOOST_CHECK_EQUAL(true, client.query(query, query_result_handler));

        BOOST_CHECK_EQUAL(1u, rows.size());

        const query_result_row& row = rows[0];

        BOOST_CHECK_EQUAL(1u, row.size());

        BOOST_CHECK_EQUAL(row[0].first , "dummy1");
        BOOST_CHECK_EQUAL(row[0].second, "data3");

        table.create(false);
        BOOST_CHECK_EQUAL(true, client.table(table));

    }

}}} // tp::database::sqlite3_client
