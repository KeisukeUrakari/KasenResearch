#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

extern "C" {
#include <mosquitto.h>
}

class Mosquitto {
  public:
    using disconnect_t = std::function<void(const std::string &reason)>;
    using subscribe_t = std::function<void(const std::string &topic, const void *data, std::size_t size)>;

    Mosquitto();
    ~Mosquitto();

    std::string connect(const std::string &host, int port, int keepalive, disconnect_t disconnectCallback);
    void disconnect();
    std::string publish(const std::string &topic, const void *data, std::size_t size, int qos, bool retain);
    std::string publish(const std::string &topic, const std::string &message, int qos, bool retain);
    void subscribe(const std::vector<std::string> &topics, subscribe_t callback);

  private:
    struct mosquitto *mosq_;
    std::mutex mutex_;
    bool connected_;
    disconnect_t disconnectCallback_;
    subscribe_t subscribeCallback_;
};