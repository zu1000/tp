#ifndef __tp_database_client_hpp__
#define __tp_database_client_hpp__

#include <boost/noncopyable.hpp>

#include "sql_table.hpp"
#include "sql_query.hpp"
#include "sql_update.hpp"
#include "sql_insert.hpp"

namespace tp {
namespace database {

    class client : public boost::noncopyable
    {
    public:
        virtual ~client() = 0;

        virtual bool query(
                const sql_query& query, const query_result_handler&) = 0;

        virtual bool update(
                const sql_update& update) = 0;

        virtual bool insert(
                const sql_insert& insert) = 0;

        virtual bool table(
                const sql_table& table) = 0;

        virtual bool begin_transaction() = 0;

        virtual bool end_transaction() = 0;
    };

}}// tp::database

#endif//__tp_database_client_hpp__
