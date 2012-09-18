#include "application.hpp"

#include <iostream>

namespace tp {
namespace database {
namespace ticker {

    application::application()
        : base("ticker_database", logger_)
        , logger_(std::cout)
        , database_(logger_)
    {
        options_.add(
                "database_file", "d",
                boost::optional<std::string>("ticker_database"),
                "specify the database file name");
    }

    bool
    application::initialize(int argc, const char** argv)
    {
        if (!base::initialize(argc, argv))
            return false;

        std::string database_file;
        if (!options_.get("database_file", database_file))
        {
            logger_ << "fail to get database file name\n"
                    << "please  set '--database_file'";
            return false;
        }
        else
            logger_ << "using database_file '" << database_file << "'\n";

        if (!database_.open(database_file))
        {
            logger_ << "fail to open database '" << database_file << "'\n";
            return false;
        }
    }

    void
    application::service_ready(const comm::service::service& service)
    {
    }

    void
    application::service_changed(const comm::service::service& service)
    {
    }

    void
    application::service_deleted(const comm::service::service_id& id)
    {
    }

}}} // tp::database::ticker


