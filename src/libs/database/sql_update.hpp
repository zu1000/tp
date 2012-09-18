#ifndef __tp_database_sql_update_hpp__
#define __tp_database_sql_update_hpp__

#include <string>
#include <vector>


namespace tp {
namespace database {

    typedef std::pair<std::string, std::string> update_condition;
    typedef std::pair<std::string, std::string> update_key_value;

    class sql_update
    {
    public:
        bool set_table(
                const std::string& table_name);

        bool set_update_key_value(
                const update_key_value& key_value,
                bool append=true);

        bool set_condition(
                const update_condition& condition,
                bool append=true,
                bool and_mode=true);

        bool sql_string(std::string& sql) const;

    private:
        std::string              table_;
        std::vector<std::string> key_values_;
        std::vector<std::string> conditions_;
    };

}} // tp::database
#endif//__tp_database_sql_update_hpp__
