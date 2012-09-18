#ifndef __tp_database_sqlite3_client_hpp__
#define __tp_database_sqlite3_client_hpp__

#include "client.hpp"

#include <string>

extern "C" {
class sqlite3;
}

namespace tp {
namespace database {

    class sqlite3_client : public client
    {
    public:
        // throw runtime_error:
        //      if database name not point to a valid database
        sqlite3_client(const std::string& connection_name);
        ~sqlite3_client();

        bool query(
                const sql_query& query,
                const query_result_handler& handler);

        bool update(
                const sql_update& update);

        bool insert(
                const sql_insert& insert);

        bool table(
                const sql_table& table);

        bool begin_transaction();

        bool end_transaction();

    private:
        sqlite3* database_connection_;
    };

}}// tp::database

#endif//__tp_database_sqlite3_client_hpp__
