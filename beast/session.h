#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <memory>
#include <thread>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>

// Handles an HTTP server connection
class Session : public std::enable_shared_from_this<Session> {
  protected:
    using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

  public:
    // Take ownership of the stream
    Session(tcp::socket &&socket)
        : lambda_(*this), stream_(std::move(socket)) {
    }

    void run();
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void on_write(bool close, beast::error_code ec, std::size_t bytes_transferred);
    void do_close();

  protected:
    virtual void handle_request(http::request<http::string_body> &&req);
    //    void handle_request();

    struct send_lambda {
        Session &self_;

        explicit send_lambda(Session &self)
            : self_(self) {
        }

        template <bool isRequest, class Body, class Fields>
        void operator()(http::message<isRequest, Body, Fields> &&msg) const {
            // The lifetime of the message has to extend
            // for the duration of the async operation so
            // we use a shared_ptr to manage it.
            auto sp = std::make_shared<http::message<isRequest, Body, Fields>>(std::move(msg));

            // Store a type-erased version of the shared
            // pointer in the class to keep it alive.
            self_.res_ = sp;

            // Write the response
            http::async_write(
                self_.stream_,
                *sp,
                beast::bind_front_handler(
                    &Session::on_write,
                    self_.shared_from_this(),
                    sp->need_eof()));
        }
    };

    send_lambda lambda_;

  private:
    // This is the C++11 equivalent of a generic lambda.
    // The function object is used to send an HTTP message.
    void fail(beast::error_code ec, char const *);

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    std::shared_ptr<void> res_;
};
