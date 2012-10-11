#include <comm/io/peer.hpp>
#include <comm/io/acceptor.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <iostream>

using namespace tp::comm;

std::vector< boost::shared_ptr<io::peer> > peers;

std::size_t handle_message(const io::const_buffer& buff, std::size_t size)
{
    std::string str(boost::asio::buffer_cast<const char*>(buff), size);
    std::cout << str << std::endl;
    return size;
}

bool handle_error(const io::error_code& error)
{
    return true;
}

void handle_peer(boost::shared_ptr<io::peer> p)
{
    std::cout << "get new peer: " << p->peer_address() << std::endl;
    p->init_peer(handle_message, handle_error);
    peers.push_back(p);
}

int main(void)
{
    boost::asio::io_service io;
    service::service s(-1, service::service::TCP, "127.0.0.1", 10020, true);

    io::acceptor service(io, s, handle_peer);

    io.run();

    return 0;
}
