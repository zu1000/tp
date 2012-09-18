#ifndef __tp_database_ticker_ticker_database_hpp__
#define __tp_database_ticker_ticker_database_hpp__

#include "database.hpp"

#include <application/base.hpp>
#include <utils/simple_logger.hpp>

namespace tp {
namespace database {
namespace ticker {

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

}}} // tp::database::ticker

#endif//__tp_database_ticker_ticker_database_hpp__
