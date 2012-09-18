#include "session_configuration.hpp"

#include <stdexcept>

namespace tp {
namespace gateways {
namespace shfe {

    session_configuration::session_configuration(
            const std::string& shfe_configuration_file)
    {
        application::options options("shfe_config");

        if (!options.parse(shfe_configuration_file))
            throw std::invalid_argument(
                    "invalid shfe configuration file: " +
                    shfe_configuration_file);

        get_mandatory_option(
                options, "front_gateway", front_);
        get_mandatory_option(
                options, "broker_id", broker_id_);
        get_mandatory_option(
                options, "user_id", user_id_);
        get_mandatory_option(
                options, "password", password_);

        get_optional_option(
                options, "trading_day", trading_day_);
        get_optional_option(
                options, "user_product_info", user_product_info_);
        get_optional_option(
                options, "interface_product_info", interface_product_info_);
        get_optional_option(
                options, "protocol_info", protocol_info_);
        get_optional_option(
                options, "mac_address", mac_address_);
        get_optional_option(
                options, "one_time_password", one_time_password_);
        get_optional_option(
                options, "client_ip_address", client_ip_address_);
    }

}}} // tp::gateways::shfe
