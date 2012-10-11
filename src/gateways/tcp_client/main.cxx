#include <comm/io/sender_receiver.hpp>
#include <comm/service/service.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <iostream>

using namespace tp::comm;

std::size_t handle_message(const io::const_buffer& buff, std::size_t size)
{
    std::string str(boost::asio::buffer_cast<const char*>(buff), size);
    std::cout << str << std::endl;
    return size;
}

bool handle_error(const io::error_code& error)
{
}

int main(int argc, const char** argv)
{
    if (argc != 3 || !atoi(argv[1]))
    {
        std::cout << "specify valid port please" << std::endl;
        return 1;
    }

    boost::asio::io_service io;
    service::service s(-1, service::service::TCP, "127.0.0.1", atoi(argv[1]), true);

    io::sender_receiver sr(s, io, handle_message, handle_error);

    std::string msg(argv[2], strlen(argv[2]));
    msg += "\n";

    io::error_code error;
    sr.send(io::const_buffer(msg.c_str(), msg.length()), error);

    io.run();

    return 0;
}
