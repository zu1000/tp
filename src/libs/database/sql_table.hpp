#ifndef __tp_database_sql_table_hpp__
#define __tp_database_sql_table_hpp__

#include <string>

#include <vector>

namespace tp {
namespace database {

    class sql_table
    {
    public:
        sql_table(const std::string& name, bool create=true);

        void set_name(const std::string& name) { name_ = name; }

        bool set_column(
                const std::string& column,
                bool append=true);

        bool sql_string(std::string& sql) const;

        void create(bool create) { create_ = create; }

        bool create() const { return create_; }

        const std::string& name() const { return name_; }

    private:
        bool                     create_;
        std::string              name_;
        std::vector<std::string> columns_;
    };

}}//tp::database

#endif//__tp_database_sql_table_hpp__
