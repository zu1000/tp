#ifndef __tp_database_indicator_indicator_database_hpp__
#define __tp_database_indicator_indicator_database_hpp__

#include "database.hpp"

#include <application/base.hpp>
#include <utils/simple_logger.hpp>

#include <boost/asio/io_service.hpp>

namespace tp {
namespace database {
namespace indicator {

    class application : public tp::application::base<utils::simple_logger>
    {
        typedef tp::application::base<utils::simple_logger> base;
    public:
        application();

        bool initialize(int argc, const char** argv);

    private:
        virtual void service_ready  (const comm::service::service& service);
        virtual void service_changed(const comm::service::service& service);
        virtual void service_deleted(const comm::service::service_id& id);

    private:
        utils::simple_logger logger_;
        database             database_;
    };

}}}

#endif//__tp_database_indicator_indicator_database_hpp__
