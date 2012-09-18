#ifndef __tp_gateways_shfe_session_configuration_hpp__
#define __tp_gateways_shfe_session_configuration_hpp__

#include "utils.hpp"

#include <application/options.hpp>

#include <string>

namespace tp {
namespace gateways {
namespace shfe {

    class session_configuration
    {
    public:
        session_configuration(const std::string& shfe_configration_file);

        const std::string& front() const
        { return front_; }

        const std::string& trading_day() const
        { return trading_day_; }

        const std::string& broker_id() const
        { return broker_id_; }

        const std::string& user_id() const
        { return user_id_; }

        const std::string& password() const
        { return password_; }

        const std::string& user_product_info() const
        { return user_product_info_; }

        const std::string& interface_product_info() const
        { return interface_product_info_; }

        const std::string& protocol_info() const
        { return protocol_info_; }

        const std::string& mac_address() const
        { return mac_address_; }

        const std::string& one_time_password() const
        { return one_time_password_; }

        const std::string& client_ip_address() const
        { return client_ip_address_; }

    private:
        std::string front_;
        std::string trading_day_;
        std::string broker_id_;
        std::string user_id_;
        std::string password_;
        std::string user_product_info_;
        std::string interface_product_info_;
        std::string protocol_info_;
        std::string mac_address_;
        std::string one_time_password_;
        std::string client_ip_address_;
    };

}}} // tp::gateways::shfe

#endif//__tp_gateways_shfe_session_configuration_hpp__
