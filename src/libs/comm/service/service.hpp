/*
 * Service is an abstract for the services provided in the system. A service
 * will have a id and then a description to tell the consumer how to subscribe
 * the service.
 *
 * A service can be subscribed with following way:
 *      - as a TCP connection
 *      - join a Multicast Channel
 *
 * Service id must be unique in the whole system.
 */

#ifndef __tp_comm_service_service_h__
#define __tp_comm_service_service_h__

#include "id.hpp"

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/utility.hpp>

namespace tp {
namespace comm {
namespace service {

    class service : public boost::noncopyable
    {
    public:
        enum connection_type
        {
            TCP,
            UDP,
            Multicast
        };

        /*
         * Exceptions:
         *
         *  std::runtime_error
         *      when addr is a invalid string for boost::asio::ip::address
         *
         *  std::logic_error
         *      when addr type is TCP but addr is a multicast address
         *      or addr type is Multicast but addr is Non-Multicast address
         */
        service(const service_id&       id,
                const connection_type&  type,
                const std::string&      addr,
                const uint32_t          port,
                bool                    is_provider,
                const std::string&      if_addr = "");

        /*
         * Exceptions:
         *
         *  std::runtime_error
         *      when addr is a invalid string for boost::asio::ip::address
         *
         *  std::logic_error
         *      when addr type is TCP but addr is a multicast address
         *      or addr type is Multicast but addr is Non-Multicast address
         */
        service(const service_id&       id,
                const connection_type&  type,
                const boost::asio::ip::address& addr,
                const uint32_t          port,
                bool                    is_provider,
                const boost::asio::ip::address& if_addr = boost::asio::ip::address());

        const service_id& id() const
        {
            return id_;
        }

        const connection_type type() const
        {
            return type_;
        }

        const boost::asio::ip::address& addr() const
        {
            return addr_;
        }

        uint16_t port() const
        {
            return port_;
        }

        bool is_provider() const
        {
            return is_provider_;
        }

        const boost::asio::ip::address& if_addr() const
        {
            return if_addr_;
        }

    private:
        service_id               id_;
        connection_type          type_;
        uint32_t                 port_;
        boost::asio::ip::address addr_;
        boost::asio::ip::address if_addr_;
        bool                     is_provider_;
    };

    typedef boost::shared_ptr<service> service_ptr;

    static inline
    bool operator < (const service_ptr& s1, const service_ptr& s2)
    {
        s1->id() < s2->id();
    }

    static inline
    bool operator == (const service& s1, const service& s2)
    {
        return s1.id() == s2.id()
            && s1.type() == s2.type()
            && s1.addr() == s2.addr()
            && s1.port() == s2.port();
    }

    static inline
    bool operator != (const service& s1, const service& s2)
    {
        return !(s1 == s2);
    }

    // Using unordered map instead of std::map, since... the service
    // information is not changed frequently, the order doesn't matter.
    typedef boost::unordered_map<service_id, service_ptr> service_id_to_ptr_map;

    // Set of service id
    typedef boost::unordered_set<service_id>             service_id_set;

}}}//tp::comm::service

#endif//__tp_comm_service_service_h__
