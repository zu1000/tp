#include "sqlite3_client.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <stdexcept>

extern "C"{
#include <sqlite3.h>
}

namespace tp {
namespace database {

    static int sqlite_exec_handler(
            void* data, int num, char** values, char** columns)
    {
        if (num <= 0 || !values || !columns)
        {
            return 0;
        }

        query_result_row row(num);

        for (size_t i = 0; i < num; ++i)
        {
            row[i].first  = columns ? columns[i] : "";
            row[i].second = values ? values[i] : "";
        }

        const query_result_handler& handler =
            *static_cast<query_result_handler*>(data);

        if (handler(row))
            return 0;
        else
            return -1;
    }

    sqlite3_client::sqlite3_client(const std::string& connection_name)
    {
        if (SQLITE_OK !=
                sqlite3_open(connection_name.c_str(), &database_connection_))
            throw std::runtime_error(
                    "cannot open database " + connection_name +
                    "(" + sqlite3_errmsg(database_connection_) + ")");
    }

    sqlite3_client::~sqlite3_client()
    {
        if (SQLITE_OK != sqlite3_close(database_connection_))
        {
            // TODO: log a error message
        }
    }

    bool
    sqlite3_client::query(
            const sql_query& query, const query_result_handler& handler)
    {
        std::string sql;

        if (!query.sql_string(sql))
            return false;

        if (SQLITE_OK !=
                sqlite3_exec(
                    database_connection_, sql.c_str(), sqlite_exec_handler,
                    const_cast<query_result_handler*>(&handler), NULL))
        {
            return false;
        }

        return true;
    }

    bool
    sqlite3_client::update(const sql_update& update)
    {
        std::string sql;
        if (!update.sql_string(sql))
            return false;

        return SQLITE_OK ==
            sqlite3_exec(database_connection_, sql.c_str(), NULL, NULL, NULL);
    }

    bool
    sqlite3_client::insert(const sql_insert& insert)
    {
        std::string sql;
        if (!insert.sql_string(sql))
            return false;

        return SQLITE_OK ==
            sqlite3_exec(database_connection_, sql.c_str(), NULL, NULL, NULL);

        return true;
    }

    static int check_table(void* data, int, char**, char**)
    {
        *static_cast<bool*>(data) = true;
        return 0;
    }

    bool
    sqlite3_client::table(const sql_table& table)
    {
        std::string sql;

        // check the existance of the table
        sql = "select name from sqlite_master where type='table' and name='"
            + table.name() + "'";

        bool table_exist = false;
        if (SQLITE_OK !=
                sqlite3_exec(
                    database_connection_, sql.c_str(),
                    check_table, &table_exist, NULL))
            table_exist = false;

        if (!table.sql_string(sql))
            return false;

        return !(table_exist ^ table.create()) ||
            SQLITE_OK == sqlite3_exec(
                    database_connection_, sql.c_str(), NULL, NULL, NULL);

        //const char* msg = sqlite3_errmsg(database_connection_);

        //return SQLITE_OK == res;
    }

    bool
    sqlite3_client::begin_transaction()
    {
        return SQLITE_OK == sqlite3_exec(
                database_connection_,"BEGIN TRANSACTION", NULL, NULL, NULL);
    }

    bool
    sqlite3_client::end_transaction()
    {
        return SQLITE_OK == sqlite3_exec(
                database_connection_,"END TRANSACTION", NULL, NULL, NULL);
    }

}} // tp::database
