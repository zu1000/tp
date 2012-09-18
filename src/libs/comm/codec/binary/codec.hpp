#ifndef __tp_comm_codec_binary_codec_hpp__
#define __tp_comm_codec_binary_codec_hpp__

#include <boost/asio/buffer.hpp>
#include <boost/utility.hpp>

#include <cstring>

extern "C"
{
#include <stdint.h>
}


namespace tp {
namespace comm {
namespace codec {
namespace binary {

    template <typename P>
    class codec : public boost::noncopyable
    {
    public:
        typedef typename P::msg_header_type msg_header_type;
        typedef typename P::msg_handler_type msg_handler_type;
        codec(msg_handler_type& handler)
            : handler_(handler)
        {}

        template<typename T>
        inline std::size_t
        encode(
                const T& src,
                boost::asio::mutable_buffer& buf);

        inline std::size_t
        decode(
                const boost::asio::const_buffer& buf,
                std::size_t bytes_transferred);

    private:
        msg_handler_type& handler_;
    };

    template <typename P>
    template <typename T>
    inline std::size_t
    codec<P>::encode(
            const T& src, boost::asio::mutable_buffer& buf)
    {
        std::size_t buf_size = boost::asio::buffer_size(buf);

        if (sizeof(T) > buf_size)
            return 0;

        std::memcpy(boost::asio::buffer_cast<void*>(buf), &src, sizeof(T)); 

        buf = buf + sizeof(T);

        return sizeof(T);
    }

    template <typename P>
    inline std::size_t
    codec<P>::decode(
            const boost::asio::const_buffer& buf,
            std::size_t bytes_transferred)
    {
        std::size_t buf_size = bytes_transferred;

        if (buf_size < P::min_pkt_len)
            return 0;

        std::size_t decoded_size = 0u, decoded_pkt_size = 0u, pkt_len = 0u;

        const char* msg = boost::asio::buffer_cast<const char*>(buf);

        do
        {
            decoded_pkt_size = 0u;
            decoded_pkt_size += P::check_message(msg, pkt_len);

            while (pkt_len && pkt_len <= buf_size && decoded_pkt_size < pkt_len)
            {
                size_t size = handler_.handle_message(
                        reinterpret_cast<const msg_header_type*>(msg+decoded_size+decoded_pkt_size));

                if (!size) return (decoded_size + decoded_pkt_size);

                decoded_pkt_size += size;
            }

            decoded_size += decoded_pkt_size;

        } while(decoded_pkt_size && decoded_size < buf_size);

        return decoded_size;
    }

}}}} // tp::comm::codec::binary

#endif//__tp_comm_codec_binary_codec_hpp__
