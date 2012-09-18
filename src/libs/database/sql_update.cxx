#include "sql_update.hpp"

namespace tp {
namespace database {

    bool
    sql_update::set_table(
            const std::string& table_name)
    {
        if (table_name.empty())
            return false;

        table_ = table_name;

        return true;
    }

    bool
    sql_update::set_update_key_value(
            const update_key_value& key_value, bool append)
    {
        if (key_value.first.empty() || key_value.second.empty())
            return false;

        if (!append)
            key_values_.clear();

        key_values_.push_back(key_value.first + "='" + key_value.second + "'");

        return true;
    }

    bool
    sql_update::set_condition(
            const update_condition& condition, bool append, bool and_mode)
    {
        if (condition.first.empty() || condition.second.empty())
            return false;

        if (!append)
            conditions_.clear();

        if (!conditions_.empty())
            conditions_.push_back((and_mode?"AND":"OR"));

        conditions_.push_back(condition.first + "='" + condition.second + "'");

        return true;
    }

    bool
    sql_update::sql_string(std::string& sql) const
    {
        if (table_.empty() || key_values_.empty())
            return false;

        sql = "update '" + table_ + "' set ";

        size_t size = key_values_.size();
        for (size_t i = 0; i < size; ++i)
        {
            sql += key_values_[i];
            if (size -1 != i)
                sql += ",";
        }

        size = conditions_.size();

        if (size)
        {
            sql += " where";

            for (size_t i = 0; i < size; ++i)
                sql += " " + conditions_[i];
        }

        sql += ";";

        return true;
    }

}} // tp::database
