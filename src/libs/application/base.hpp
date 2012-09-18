#ifndef __tp_database_base_hpp__
#define __tp_database_base_hpp__

#include "options.hpp"

#include <comm/service/event_listener.hpp>
#include <comm/service/simple_manager.hpp>

#include <boost/noncopyable.hpp>
#include <boost/asio/io_service.hpp>

namespace tp {
namespace application {

    template <typename logger>
    class base : public comm::service::event_listener
    {
    public:
        base(const std::string& app_name, logger& l);
        virtual ~base() = 0;

        void run() { io_.run(); }

        virtual bool initialize(int argc, const char** argv);

    protected:
        options                           options_;
        boost::asio::io_service           io_;
        comm::service::simple_manager_ptr service_manager_;
        comm::service::configuration_ptr  configuration_;

    private:
        logger&                           logger_;
    };

    template <typename logger>
    inline
    base<logger>::base(const std::string& app_name, logger& l)
        : options_(app_name)
        , logger_(l)
    {
        options_.add(
                "help", "h", "print this option help text");

        options_.add(
                "config_file", "f", boost::optional<std::string>(),
                "specify the location/name of application config file");

        options_.add(
                "service_file", "s", boost::optional<std::string>(),
                "specify the location/name of service config file");

    }

    template <typename logger>
    inline
    base<logger>::~base()
    {}

    template <typename logger>
    inline bool
    base<logger>::initialize(int argc, const char** argv)
    {
        if (!options_.parse(argc, argv) || options_.has("help"))
        {
            logger_ << options_.help();
            return false;
        }

        std::string config_file_name;
        if (!options_.get("config_file", config_file_name) ||
            config_file_name.empty())
        {
            logger_ << "You must specify option --config_file\n";
            return false;
        }

        if (!options_.parse(config_file_name))
        {
            logger_ << "fail to parse config_file " << config_file_name << "\n";
            return false;
        }

        std::string service_file;
        if (!options_.get("service_file", service_file))
        {
            logger_ << "fail to get service manager config name\n"
                    << "please set '--service_file'\n";
            return false;
        }
        else
        {
            try
            {
                configuration_.reset(new comm::service::configuration(service_file));
                service_manager_.reset(
                        new comm::service::simple_manager(*configuration_, *this));
            }
            catch (const std::exception& e)
            {
                logger_ << "fail to configure service. (" << e.what() << ")\n";
                return false;
            }
        }

        return true;
    }


}} // tp::database

#endif//__tp_database_base_hpp__
