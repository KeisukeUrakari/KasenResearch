#pragma once

#include "session.h"

class RoadSession : public Session {
public:
    RoadSession(tcp::socket &&socket)
        : Session(std::move(socket)) {
    }

protected:
    void handle_request(http::request<http::string_body> &&req) override;

};