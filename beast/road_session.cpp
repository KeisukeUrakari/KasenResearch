#include <boost/log/trivial.hpp>

#include "road_session.h"

void RoadSession::handle_request(http::request<http::string_body> &&req) {
    BOOST_LOG_TRIVIAL(trace) << "handle request";
    BOOST_LOG_TRIVIAL(trace) << "method:" << req.method() << " : " << req.target();
    BOOST_LOG_TRIVIAL(trace) << "method:" << req.body();

    std::chrono::milliseconds dura(2000);
    std::this_thread::sleep_for(dura);
    BOOST_LOG_TRIVIAL(trace) << "handle request wait end";


    if(req.target() == "/index.json") {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = std::string("{\"key\":123}");
        return lambda_(std::move(res));
    }

    session::handle_request(std::move(req));
}
