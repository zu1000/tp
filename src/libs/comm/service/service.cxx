#include "service.hpp"

#include <stdexcept>

namespace tp {
namespace comm {
namespace service {

using namespace boost::asio;

// This is not implemented as a member function since this should no disclosed
// to user of this class.
static bool validate_addr_type(
        const service::connection_type& type, const ip::address& addr)
{
    bool is_multicast = false;

    if (addr.is_v4())
    {
        is_multicast = addr.to_v4().is_multicast();
    }
    else
    if (addr.is_v6())
    {
        is_multicast = addr.to_v6().is_multicast();
    }

    switch(type)
    {
    case service::TCP:
    case service::UDP:
        return !is_multicast;
    case service::Multicast:
        return is_multicast;
    }

    return false;
}

service::service(
        const service_id&       id,
        const connection_type&  type,
        const std::string&      addr,
        const uint32_t          port,
        bool                    is_provider,
        const std::string&      if_addr)
    : id_(id)
    , type_(type)
    , port_(port)
    , addr_(ip::address::from_string(addr))
    , if_addr_()
    , is_provider_(is_provider)
{
    if (!validate_addr_type(type_, addr_))
        throw std::logic_error("addr type mismatch");

    if (!if_addr.empty())
        if_addr_ = ip::address::from_string(if_addr);
}

service::service(
        const service_id&       id,
        const connection_type&  type,
        const boost::asio::ip::address& addr,
        const uint32_t          port,
        bool                    is_provider,
        const boost::asio::ip::address& if_addr)
    : id_(id)
    , type_(type)
    , port_(port)
    , addr_(addr)
    , if_addr_(if_addr)
    , is_provider_(is_provider)
{
    if (!validate_addr_type(type_, addr_))
        throw std::logic_error("addr type mismatch");
}

}}}//tp::comm::service
