#include <boost/log/trivial.hpp>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>

#include "../mosquitto.h"

Mosquitto::Mosquitto() : mosq_(nullptr), connected_(false) {
    if(MOSQ_ERR_SUCCESS != mosquitto_lib_init()) {
        assert(false);
    }

    mosq_ = mosquitto_new(nullptr, true, this);
    if(!mosq_) {
        assert(false);
    }

    mosquitto_connect_callback_set(mosq_, [](struct mosquitto *mosq, void *obj, int rc) {
        const auto m = reinterpret_cast<Mosquitto *>(obj);

        if(!rc) {
            std::lock_guard<std::mutex> lock(m->mutex_);
            m->connected_ = true;
        }
    });

    mosquitto_disconnect_callback_set(mosq_, [](struct mosquitto *mosq, void *obj, int rc) {
        const auto m = reinterpret_cast<Mosquitto *>(obj);
        {
            std::lock_guard<std::mutex> lock(m->mutex_);
            m->connected_ = false;
        }
        m->disconnectCallback_(rc ? mosquitto_strerror(rc) : "");
    });

    mosquitto_message_callback_set(mosq_, [](struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
        const auto topic = std::string(message->topic);
        const auto m = reinterpret_cast<Mosquitto *>(obj);

        if(message->payloadlen > 0) {
            m->subscribeCallback_(topic, message->payload, std::size_t(message->payloadlen));
        }
    });

    mosquitto_loop_start(mosq_);
}

Mosquitto::~Mosquitto() {
    mosquitto_loop_stop(mosq_, true);
    mosquitto_lib_cleanup();
}

std::string Mosquitto::connect(const std::string &host, int port, int keepalive, disconnect_t disconnectCallback) {
    disconnectCallback_ = disconnectCallback;

    const auto err = mosquitto_connect(mosq_, host.c_str(), port, keepalive);
    if(MOSQ_ERR_SUCCESS != err) {
        return std::string(mosquitto_strerror(err));
    }

    return "";
}

void Mosquitto::disconnect() {
    mosquitto_disconnect(mosq_);
}

std::string Mosquitto::publish(const std::string &topic, const void *data, std::size_t size, int qos, bool retain) {
    const auto err = mosquitto_publish(mosq_, nullptr, topic.c_str(), size, data, qos, retain);
    return (MOSQ_ERR_SUCCESS == err) ? "" : mosquitto_strerror(err);
}

std::string Mosquitto::publish(const std::string &topic, const std::string &message, int qos, bool retain) {
    return publish(topic, message.data(), message.size(), qos, retain);
}

void Mosquitto::subscribe(const std::vector<std::string> &topics, subscribe_t callback) {
    for(const auto &t : topics) {
        mosquitto_subscribe(mosq_, nullptr, t.c_str(), 0);
    }
    subscribeCallback_ = callback;
}
