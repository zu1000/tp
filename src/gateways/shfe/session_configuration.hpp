#ifndef __tp_gateways_shfe_session_configuration_hpp__
#define __tp_gateways_shfe_session_configuration_hpp__

namespace tp {
namespace gateways {
namespace shfe {

    class session_configuration
    {
    public:
        session_configuration(
                const std::string& front,
                const std::string& trading_day,
                const std::string& broker_id,
                const std::string& user_id,
                const std::string& password,
                const std::string& user_product_info,
                const std::string& interface_product_info,
                const std::string& protocol_info,
                const std::string& mac_address,
                const std::string& one_time_password,
                const std::string& client_ip_address)
            : front_(front)
            , trading_day_(trading_day)
            , broker_id_(broker_id)
            , user_id_(user_id)
            , password_(password)
            , user_product_info_(user_product_info)
            , interface_product_info_(interface_product_info)
            , protocol_info_(protocol_info)
            , mac_address_(mac_address)
            , one_time_password_(one_time_password)
            , client_ip_address_(client_ip_address)
        {}

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
        const std::string front_;
        const std::string trading_day_;
        const std::string broker_id_;
        const std::string user_id_;
        const std::string password_;
        const std::string user_product_info_;
        const std::string interface_product_info_;
        const std::string protocol_info_;
        const std::string mac_address_;
        const std::string one_time_password_;
        const std::string client_ip_address_;
    };

}}} // tp::gateways::shfe

#endif//__tp_gateways_shfe_session_configuration_hpp__
