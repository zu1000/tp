#include "receiver.hpp"

#include "impl/tcp/receiver.hpp"
#include "impl/udp/receiver.hpp"

#include <comm/service/service.hpp>

#include <boost/bind.hpp>

namespace tp {
namespace comm {
namespace io {

    inline std::size_t max(const std::size_t& l, const std::size_t& r)
    {
        return l > r ? l : r;
    }

    receiver::receiver(
            const service::service& service,
            boost::asio::io_service& io,
            const receive_handler& r_handler,
            const error_handler& e_handler)
    {
        const receive_handler&  rh =
            boost::bind(&receiver::async_receive_handler, this, _1, _2);

        const error_handler& eh =
            boost::bind(&receiver::async_error_handler, this, _1);

        switch (service.type())
        {
        case service::service::TCP:
            receiver_.reset(new impl::tcp::receiver(service, io, rh, eh));
            break;
        case service::service::UDP:
        case service::service::Multicast:
            receiver_.reset(new impl::udp::receiver(service, io, rh, eh));
            break;
        }

        r_handlers_.push_back(r_handler);
        e_handlers_.push_back(e_handler);
    }

    std::size_t
    receiver::register_receive_handler(
            const receive_handler& r_handler)
    {
        r_handlers_.push_back(r_handler);
        return r_handlers_.size();
    }

    std::size_t
    receiver::register_error_handler(
            const error_handler& e_handler)
    {
        e_handlers_.push_back(e_handler);
        return e_handlers_.size();
    }

    std::size_t
    receiver::async_receive_handler(
            const const_buffer& buffer,
            std::size_t bytes_transferred)
    {
        std::size_t consumed = 0u;
        for (int idx = 0; idx < r_handlers_.size(); ++idx)
            consumed =
                max(consumed, r_handlers_[idx](buffer, bytes_transferred));
    }

    bool
    receiver::async_error_handler(
            const error_code& ec)
    {
        bool res = true;

        for (int idx = 0; idx < e_handlers_.size(); ++idx)
            res = res && e_handlers_[idx](ec);

        return res;
    }

}}} //tp::comm::io
