#include "database.hpp"

#include <database/sqlite3_client.hpp>

namespace tp {
namespace database {
namespace indicator {

    bool
    database::open(const std::string name)
    {
        client_.reset(new sqlite3_client(name));
    }

}}}
