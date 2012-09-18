#include "sql_table.hpp"

namespace tp {
namespace database {

    sql_table::sql_table(const std::string& name, bool create)
        : create_(create)
        , name_(name)
    {}

    bool
    sql_table::set_column(const std::string& column, bool append)
    {
        if (column.empty())
            return false;

        if (!append)
            columns_.clear();

        columns_.push_back(column);
    }

    // Type of all table columns are string
    bool
    sql_table::sql_string(std::string& sql) const
    {
        if (name_.empty())
            return false;

        if (create_)
        {
            if (columns_.empty())
                return false;

            sql = "create table '" + name_ + "'";

            sql += " (";

            for (size_t i = 0; i < columns_.size(); ++i)
            {
                sql += columns_[i] + " varchar(40)";

                if (i < columns_.size() - 1)
                    sql += ",";
            }

            sql += ");";
        }
        else
        {
            sql = "drop table '" + name_ + "';";
        }

        return true;
    }

}}//tp::database
