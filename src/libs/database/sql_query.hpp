#ifndef __tp_database_sql_query_hpp__
#define __tp_database_sql_query_hpp__

#include <boost/function.hpp>

#include <string>
#include <vector>

namespace tp {
namespace database {

    typedef std::pair<std::string, std::string> query_condition;

    class sql_query
    {
    public:
        bool set_table(
                const std::string& table_name,
                bool append=true);

        bool set_return_column(
                const std::string& column_name,
                bool append=true);

        bool set_condition(
                const query_condition& condition,
                bool append=true,
                bool and_mode=true);

        bool sql_string(std::string& sql) const;

    private:
        std::vector<std::string> tables_;
        std::vector<std::string> columns_;
        std::vector<std::string> conditions_;
    };

    typedef std::vector<std::pair<std::string, std::string> > query_result_row;

    typedef boost::function<bool(const query_result_row&)> query_result_handler;

}} //tp::database

#endif//__tp_database_sql_query_hpp__
