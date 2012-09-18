#ifndef __tp_comm_service_serailize_hpp__
#define __tp_comm_service_serailize_hpp__

#include "service.hpp"

#include <comm/protocol/internal/service.hpp>

#include <boost/noncopyable.hpp>

namespace tp {
namespace comm {
namespace service {
    class service;

    template <typename B>
    class serializer : public boost::noncopyable
    {
    public:
        static bool serialize(
                const service& service,
                protocol::internal::service_t& s);

        static bool deserialize(
                service_ptr& ptr,
                const protocol::internal::service_t& buf);
    };

    template <typename B>
    inline bool
    serializer<B>::serialize(
            const service& service,
            protocol::internal::service_t& s)
    {
        s.id_   = B::swap(service.id());
        s.type_ = B::swap(uint32_t(service.type()));
        s.port_ = B::swap(service.port());

        if (service.addr().is_v4())
        {
            const boost::asio::ip::address_v4::bytes_type& bytes =
                service.addr().to_v4().to_bytes();
            size_t b_size = bytes.size();
            std::memcpy(s.addr_, bytes.begin(), b_size);

            s.version_ = protocol::internal::service_t::V4;
        }
        else
        if (service.addr().is_v6())
        {
            const boost::asio::ip::address_v6::bytes_type& bytes =
                service.addr().to_v6().to_bytes();
            size_t b_size = bytes.size();
            std::memcpy(s.addr_, bytes.begin(), b_size);

            s.version_ = protocol::internal::service_t::V6;
        }

        return true;
    }

    template <typename B>
    bool
    serializer<B>::deserialize(
            service_ptr& ptr,
            const protocol::internal::service_t& s)
    {
        switch(s.version_)
        {
            case protocol::internal::service_t::V4:
            {
                boost::asio::ip::address_v4::bytes_type bytes;
                size_t b_size = bytes.size();
                std::memcpy(bytes.begin(), s.addr_, b_size);

                boost::asio::ip::address_v4 addr(bytes);

                ptr.reset(
                        new service(
                            B::swap(s.id_),
                            service::connection_type(B::swap(s.type_)),
                            boost::asio::ip::address(addr),
                            B::swap(s.port_),
                            false));
                return true;
            }
            case protocol::internal::service_t::V6:
            {
                boost::asio::ip::address_v6::bytes_type bytes;
                size_t b_size = bytes.size();
                std::memcpy(bytes.begin(), s.addr_, b_size);

                boost::asio::ip::address_v6 addr(bytes);

                ptr.reset(
                        new service(
                            B::swap(s.id_),
                            service::connection_type(B::swap(s.type_)),
                            boost::asio::ip::address(addr),
                            B::swap(s.port_),
                            false));
                return true;
            }
            default:
            {
                // TODO: log error message here
                return false;
            }
        }
    }

}}}

#endif//__tp_comm_service_serailize_hpp__
