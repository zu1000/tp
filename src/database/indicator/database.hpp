#ifndef __tp_database_indicator_database_hpp__
#define __tp_database_indicator_database_hpp__

#include <database/client.hpp>

#include <boost/smart_ptr.hpp>

namespace tp {
namespace database {
namespace indicator {

    class database
    {
    public:
        bool open(const std::string name);

    private:
        boost::scoped_ptr<client> client_;
    };

}}} // tp::database::indicator
#endif//__tp_database_indicator_database_hpp__
