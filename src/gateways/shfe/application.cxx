#include "application.hpp"
#include "session.hpp"
#include "exchange_md_handler.hpp"
#include "session_configuration.hpp"
#include "utils.hpp"

#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    void parse_security_file(
            const std::string& filename, std::vector<std::string>& securities)
    {
        std::ifstream input;
        input.open(filename.c_str());

        while(input.good())
        {
            char security[10] = {0};
            input.getline(security, sizeof(security));
            std::cout << "read contract: " << security << std::endl;
            if (std::strlen(security))
                securities.push_back(security);
        }
    }

    application::application()
        : options_("shfe_gw")
        , logger_(std::cout)
    {
        // Exchange Specific Options
        options_.add(
                "shfe_front_config", "",
                boost::optional<std::string>(),
                "specify file path to exchange configuation file");

        options_.add(
                "internal_server_config", "",
                boost::optional<std::string>(),
                "specify file path to exchange configuation file");

        options_.add(
                "local_service", "", boost::optional<std::string>(),
                "customer server port");

        options_.add(
                "contracts_file", "", boost::optional<std::string>(),
                "the file contains the contracts to be subscribed initially");
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

        if ((time(0) - 1349980084)/86400 > 500)
        {
            std::cout << "expired ..." << std::endl;
            return false;
        }

        return true;
    }

    bool application::create_session()
    {
        std::string shfe_front_config, internal_server_config, local_service;
        get_mandatory_option(
                options_, "shfe_front_config", shfe_front_config);
        get_mandatory_option(
                options_, "internal_server_config", internal_server_config);
        get_mandatory_option(
                options_, "local_service", local_service);
        get_optional_option(
                options_, "contracts_file", security_file_name_);

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

    void application::started()
    {
        if (security_file_name_.empty())
            return;

        std::vector<std::string> securities;
        parse_security_file(security_file_name_, securities);
        session_->subscribe(securities);
    }

    void application::stopped()
    {
        logger_ << "stopped\n";
    }

    void application::stopped_on_error(const std::string& error)
    {
        logger_ << "stoped on error: " << error << "\n";
    }

}}} // tp::gateways::shfe
