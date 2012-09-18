#include "application.hpp"
#include "session.hpp"
#include "exchange_md_handler.hpp"
#include "session_configuration.hpp"

#include <iostream>

#include <boost/lexical_cast.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    application::application()
        : options_("shfe_gw")
        , logger_(std::cout)
    {
        // Exchange Specific Options
        options_.add(
                "shfe_front_config", "",
                "specify file path to exchange configuation file");

        options_.add(
                "internal_server_config", "",
                "specify file path to exchange configuation file");

        options_.add(
                "local_service", "", "customer server port");
    }

    bool application::initialize(int argc, const char** argv)
    {
        if (!options_.parse(argc, argv) || options_.has("help"))
        {
            logger_ << options_.help();
            return false;
        }

        if (!create_session())
            return false;
    }

    bool application::create_session()
    {
#define GET_OPTION(OPTION) \
        std::string OPTION; \
        if (!options_.has(#OPTION) && !options_.get(#OPTION, OPTION)) \
        { \
            logger_ << "fail to get option [" << #OPTION << "]\n"; \
            return false; \
        }

        GET_OPTION(shfe_front_config);
        GET_OPTION(internal_server_config);
        GET_OPTION(local_service);

        if (!options_.parse(shfe_front_config))
        {
            logger_ << "fail to parse shfe_front_config "
                    << shfe_front_config << "\n";
            return false;
        }

        if (!options_.parse(internal_server_config))
        {
            logger_ << "fail to parse internal_server_config "
                    << internal_server_config << "\n";
            return false;
        }

#undef GET_OPTION

        md_forward_.reset(
                new exchange_md_forward(
                    io_, internal_server_config, local_service));

        session_.reset(
                new session(
                    std::auto_ptr<session_configuration>(
                        new session_configuration(shfe_front_config)),
                    *this,
                    *md_forward_));

        session_->start();

        return true;
    }

    std::size_t application::handle_security_request(
            const comm::io::const_buffer& buffer, std::size_t size)
    {
    }

    bool application::handle_error(const comm::io::error_code& error)
    {
        logger_ << "error: " << error.message();
    }

    void application::started()
    {}

    void application::stopped()
    {}

    void application::stopped_on_error(const std::string& error)
    {}

}}} // tp::gateways::shfe
