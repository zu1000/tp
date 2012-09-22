#ifndef __tp_gateways_shfe_application_hpp__
#define __tp_gateways_shfe_application_hpp__

#include "session_state_handler.hpp"
#include "session.hpp"
#include "exchange_md_forward.hpp"

#include <comm/service/service.hpp>
#include <comm/io/sender_receiver.hpp>
#include <application/options.hpp>

#include <boost/asio/io_service.hpp>

#include <iostream>

namespace tp {
namespace gateways {
namespace shfe {

    class session;
    class exchange_md_forward;

    class application : public session_state_handler
    {
    public:
        application();

        bool initialize(int argc, const char** argv);

        void run() { io_.run(); }

    private:
        bool create_session();

        std::size_t handle_security_request(
                const comm::io::const_buffer& buffer, std::size_t size);

        bool handle_error(const comm::io::error_code& error);

        virtual void started();
        virtual void stopped();
        virtual void stopped_on_error(const std::string& error);

    private:
        tp::application::options options_;
        boost::asio::io_service io_;
        std::ostream& logger_;
        boost::scoped_ptr<comm::io::sender_receiver> sender_receiver_;
        boost::scoped_ptr<exchange_md_forward> md_forward_;
        boost::scoped_ptr<session> session_;
        std::string security_file_name_;
    };

}}} // tp::gateway::shfe

#endif//__tp_gateways_shfe_application_hpp__
