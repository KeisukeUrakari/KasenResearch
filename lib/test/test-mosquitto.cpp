#include <boost/log/trivial.hpp>
#include <cstring>
#include <gtest/gtest.h>

#include "../mosquitto.h"

class MosquittoTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

#if 0
TEST_F(MosquittoTest, test) {
    auto mosq = Mosquitto();

    const auto err = mosq.connect("127.0.0.1", 1883, 20, [](const std::string &reason) {
        BOOST_LOG_TRIVIAL(trace) << reason;
    });
    ASSERT_EQ("", err);

    const auto topics = std::vector<std::string>({"hoge", "topic"});
    auto cnt = int(0);
    mosq.subscribe(topics, [&](const std::string &topic, const void *data, std::size_t size) {
        cnt++;
        auto buf = std::string(BUFSIZ, '\0');
        std::memcpy(buf.data(), data, size);
        BOOST_LOG_TRIVIAL(trace) << buf;

        mosq.publish("topic2", (void *)"漢字", 6, 1, false);
        //        mosq.publish("topic2", "漢字", 1, false);
    });

    for(; cnt < 2;) {
        std::chrono::milliseconds dura(100);
        std::this_thread::sleep_for(dura);
    }

    std::chrono::milliseconds dura(100);
    std::this_thread::sleep_for(dura);

    mosq.disconnect();
}
#endif