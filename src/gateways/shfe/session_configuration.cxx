#include "session_configuration.hpp"

#include <stdexcept>

namespace tp {
namespace gateways {
namespace shfe {

    session_configuration::session_configuration(
            const std::string& shfe_configuration_file)
    {
        const char* front_gateway          = "front_gateway";
        const char* broker_id              = "broker_id";
        const char* user_id                = "user_id";
        const char* password               = "password";
        const char* trading_day            = "trading_day";
        const char* user_product_info      = "user_product_info";
        const char* interface_product_info = "interface_product_info";
        const char* protocol_info          = "protocol_info";
        const char* mac_address            = "mac_address";
        const char* one_time_password      = "one_time_password";
        const char* client_ip_address      = "client_ip_address";

        application::options options("shfe_config");

        add_option<std::string>(options, front_gateway);
        add_option<std::string>(options, broker_id);
        add_option<std::string>(options, user_id);
        add_option<std::string>(options, password);
        add_option<std::string>(options, trading_day);
        add_option<std::string>(options, user_product_info);
        add_option<std::string>(options, interface_product_info);
        add_option<std::string>(options, protocol_info);
        add_option<std::string>(options, mac_address);
        add_option<std::string>(options, one_time_password);
        add_option<std::string>(options, client_ip_address);

        if (!options.parse(shfe_configuration_file))
            throw std::invalid_argument(
                    "invalid shfe configuration file: " +
                    shfe_configuration_file);

        get_mandatory_option(
                options, front_gateway, front_);
        get_mandatory_option(
                options, broker_id, broker_id_);
        get_mandatory_option(
                options, user_id, user_id_);
        get_mandatory_option(
                options, password, password_);

        get_optional_option(
                options, trading_day, trading_day_);
        get_optional_option(
                options, user_product_info, user_product_info_);
        get_optional_option(
                options, interface_product_info, interface_product_info_);
        get_optional_option(
                options, protocol_info, protocol_info_);
        get_optional_option(
                options, mac_address, mac_address_);
        get_optional_option(
                options, one_time_password, one_time_password_);
        get_optional_option(
                options, client_ip_address, client_ip_address_);
    }

}}} // tp::gateways::shfe
