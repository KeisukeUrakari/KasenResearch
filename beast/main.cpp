#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#if 0 
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
#else
namespace beast = boost::beast;
using tcp = boost::asio::ip::tcp;
#endif

void do_session(tcp::socket &socket) {
    beast::error_code ec;

    beast::flat_buffer buffer;
    beast::http::request<beast::http::string_body> req;
    beast::http::read(socket, buffer, req, ec);
    if(ec) {
        //        return fail(ec, "read");
        throw std::runtime_error(ec.message());
    }

    std::cout << "method: " << req.method() << " path: " << req.target() << std::endl;

    if(req.method() != boost::beast::http::verb::get) {
        return;
    }

    beast::http::response<beast::http::string_body> res{beast::http::status::ok, req.version()};
    res.set(beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(beast::http::field::content_type, "application/json");
    res.body() = "{\"key1\":\"value2\"}";
    res.keep_alive(req.keep_alive());
    res.prepare_payload();
    beast::http::serializer<false, beast::http::string_body> sr{res};
    beast::http::write(socket, sr, ec);

    if(ec) {
        throw std::runtime_error(ec.message());
    }

    socket.shutdown(tcp::socket::shutdown_send, ec);
}

class HttpListener {
    boost::asio::io_context &io_context_;
    tcp::acceptor acceptor_;

  public:
    HttpListener(boost::asio::io_context &io_context, const tcp::endpoint &endpoint)
        : io_context_(io_context),
          acceptor_(io_context_, endpoint) {
              ;
          }

    void start_accept() {
        auto socket = std::make_shared<tcp::socket>(io_context_);
        acceptor_.async_accept(
            *socket,
            [this, socket](const boost::system::error_code &error) {
                if(error) {
//                    std::cout << "accept failed: " << error.message() << std::endl;
                } else {
//                    std::cout << "accept correct!" << std::endl;
                    do_session(*socket);
                }
                start_accept();
            });
    }
};

int main(int argc, char *argv[]) {
    try {
        // Check command line arguments.
        if(argc != 3) {
            std::cerr << "Usage: http-server-sync <address> <port>\n"
                      << "Example:\n"
                      << "    http-server-sync 0.0.0.0 8080\n";
            return EXIT_FAILURE;
        }
        auto const address = boost::asio::ip::make_address(argv[1]);
        auto const port = static_cast<unsigned short>(std::atoi(argv[2]));

        // The io_context is required for all I/O
        boost::asio::io_context ioc{1};

        auto listener = HttpListener(ioc, {address, port});
        listener.start_accept();
        ioc.run();

    } catch(const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}