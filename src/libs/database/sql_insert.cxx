#include "sql_insert.hpp"

namespace tp {
namespace database {

    sql_insert::sql_insert(const std::string& name)
        : name_(name)
    {}

    bool
    sql_insert::set_column_value(const column_value& value, bool append)
    {
        if (value.first.empty() || value.second.empty())
            return false;

        if (!append)
            column_values_.clear();

        column_values_.push_back(value);

        return true;
    }

    bool
    sql_insert::sql_string(std::string& sql) const
    {
        if (name_.empty() || column_values_.empty())
            return false;

        sql = "insert into '" + name_ + "' ";

        std::string columns, values;

        for (size_t i = 0; i < column_values_.size(); ++i)
        {
            columns += column_values_[i].first;
            values  += "'" + column_values_[i].second + "'";

            if (i != column_values_.size() - 1)
            {
                columns += ",";
                values  += ",";
            }
        }

        sql += "(" + columns + ") values (" + values + ");";
        return true;
    }

}} // tp::database
