#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
 
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
 
void fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}
 
void do_session(tcp::socket& socket)
{
    beast::error_code ec;
 
    beast::flat_buffer buffer;
    http::request<http::string_body> req;
    http::read(socket, buffer, req, ec);
    if(ec)
        return fail(ec, "read");
 
    http::response<http::empty_body> res{http::status::ok, 11};
    res.keep_alive(false);
    res.prepare_payload();
    http::serializer<false, http::empty_body> sr{res};
    http::write(socket, sr, ec);
    if(ec)
        return fail(ec, "write");
 
    socket.shutdown(tcp::socket::shutdown_send, ec);
}
 
int main(int argc, char* argv[])
{
    try
    {
        // Check command line arguments.
        if (argc != 3)
        {
            std::cerr <<
                "Usage: http-server-sync <address> <port>\n" <<
                "Example:\n" <<
                "    http-server-sync 0.0.0.0 8080\n";
            return EXIT_FAILURE;
        }
        auto const address = net::ip::make_address(argv[1]);
        auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
 
        // The io_context is required for all I/O
        net::io_context ioc{1};
 
        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ioc, {address, port}};
        for(;;)
        {
            // This will receive the new connection
            tcp::socket socket{ioc};
 
            std::cout << "dbg1" << std::endl;

            // Block until we get a connection
            acceptor.accept(socket);
 
            std::cout << "dbg2" << std::endl;

            // Launch the session
            do_session(socket);

            std::cout << "dbg3" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}