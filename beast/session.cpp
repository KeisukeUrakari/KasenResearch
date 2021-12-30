#include <boost/log/trivial.hpp>
#include <iostream>
#include <boost/log/trivial.hpp>

#include "session.h"

// Start the asynchronous operation
void Session::run() {
    // We need to be executing within a strand to perform async operations
    // on the I/O objects in this session. Although not strictly necessary
    // for single-threaded contexts, this example code is written to be
    // thread-safe by default.
    net::dispatch(stream_.get_executor(),
                  beast::bind_front_handler(
                      &Session::do_read,
                      shared_from_this()));
}

void Session::do_read() {
    // Make the request empty before reading,
    // otherwise the operation behavior is undefined.
    req_ = {};

    // Set the timeout.
    stream_.expires_after(std::chrono::seconds(30));

    // Read a request
    http::async_read(stream_, buffer_, req_,
                     beast::bind_front_handler(
                         &Session::on_read,
                         shared_from_this()));
}

void Session::on_read(
    beast::error_code ec,
    std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    // This means they closed the connection
    if(ec == http::error::end_of_stream)
        return do_close();

    if(ec) {
        BOOST_LOG_TRIVIAL(error) << ec.message();
        return;
    }

    // Send the response
    this->handle_request(std::move(req_));
}

  void Session::handle_request(http::request<http::string_body> &&req) {
    http::response<http::string_body> res{http::status::not_found, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = "The resource '" + std::string("' was not found.");
    return lambda_(std::move(res));
}

void Session::on_write(
    bool close,
    beast::error_code ec,
    std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if(ec) {
        BOOST_LOG_TRIVIAL(error) << ec.message();
        return;
    }

    if(close) {
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        return do_close();
    }

    // We're done with the response so delete it
    res_ = nullptr;

    // Read another request
    do_read();
}

void Session::do_close() {
    // Send a TCP shutdown
    beast::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

    // At this point the connection is closed gracefully
}
