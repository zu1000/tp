#ifndef __tp_database_sql_insert_hpp__
#define __tp_database_sql_insert_hpp__

#include <string>
#include <vector>

namespace tp {
namespace database {

    typedef std::pair<std::string, std::string> column_value;

    class sql_insert
    {
    public:
        sql_insert(const std::string& name);

        bool set_column_value(
                const column_value& value,
                bool append=true);

        bool sql_string(std::string& sql) const;

    private:
        std::string               name_;
        std::vector<column_value> column_values_;
    };

}}//tp::database

#endif//__tp_database_sql_insert_hpp__
