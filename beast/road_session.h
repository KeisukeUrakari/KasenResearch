#pragma once

#include "session.h"

class RoadSession : public session {
public:
    RoadSession(tcp::socket &&socket, std::shared_ptr<std::string const> const &doc_root)
        : session(std::move(socket), doc_root) {
    }

protected:
    void handle_request(http::request<http::string_body> &&req) override;

};