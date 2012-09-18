#include "sql_query.hpp"

namespace tp {
namespace database {

    bool
    sql_query::set_table(
            const std::string& table_name, bool append)
    {
        if (table_name.empty())
            return false;

        if (!append)
            tables_.clear();

        tables_.push_back(table_name);

        return true;
    }

    bool
    sql_query::set_return_column(
            const std::string& column_name, bool append)
    {
        if (column_name.empty())
            return false;

        if (!append)
            columns_.clear();

        columns_.push_back(column_name);

        return true;
    }

    bool
    sql_query::set_condition(
            const query_condition& condition, bool append, bool and_mode)
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
    sql_query::sql_string(std::string& sql) const
    {
        if (tables_.empty() || columns_.empty())
            return false;

        sql = "select ";

        size_t size = columns_.size();
        for (size_t i = 0; i < size; ++i)
        {
            sql += columns_[i];
            if (size - 1 != i)
                sql += ",";
            else
                sql += " ";
        }

        sql += "from ";

        size = tables_.size();
        for (size_t i = 0; i < size; ++i)
        {
            sql += "'" + tables_[i] + "'";
            if (size - 1 != i)
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
