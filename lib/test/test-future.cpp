#include <boost/log/trivial.hpp>
#include <exception>
#include <future>
#include <gtest/gtest.h>
#include <thread>
#include <vector>

class FutureTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

class Wait {
  public:
    Wait(int msec) : msec_(msec) {
    }

    std::future<int> func(int value) {
        auto p = std::promise<int>();
        auto f = p.get_future();

        auto t = std::thread([](int msec, int value, std::promise<int> p) {
            std::this_thread::sleep_for(std::chrono::milliseconds(msec));
            //            p.set_value(value * 2);
            p.set_value_at_thread_exit(value * 2);
        },
                             msec_, value, std::move(p));
        t.detach();
        return f;
    }

  private:
    int msec_;
};

void watiFutures(std::vector<std::future<int>> &futures) {
    //    BOOST_LOG_TRIVIAL(trace) << "wait start";
    for(bool loop = true; loop;) {
        loop = false;
        for(auto &f : futures) {
            if(std::future_status::ready != f.wait_for(std::chrono::milliseconds(200))) {
                loop = true;
                break;
            }
        }
    }
    //    BOOST_LOG_TRIVIAL(trace) << "wait end";
}

TEST_F(FutureTest, test1) {
    auto w1 = Wait(1000);
    auto w2 = Wait(500);

    //  auto f1 = w1.func(10);
    //  auto f2 = w2.func(20);

    std::vector<std::future<int>> futures;
    futures.push_back(w1.func(10));
    futures.push_back(w2.func(20));
    watiFutures(futures);

    //    BOOST_LOG_TRIVIAL(trace) << "get start";
    //    const auto r1 = futures[0].get();
    //    const auto r2 = futures[1].get();
    //    BOOST_LOG_TRIVIAL(trace) << "get end";
}

#define _FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define FLINE(msg) std::string(_FILE) + ":" + std::to_string(__LINE__) + " " + msg

class Wait2 {
  public:
    Wait2(int msec) : msec_(msec) {
    }

    std::future<int> func(int value) {
        auto p = std::promise<int>();
        auto f = p.get_future();

        auto t = std::thread([](int msec, int value, std::promise<int> p) {
            std::this_thread::sleep_for(std::chrono::milliseconds(msec));
            p.set_exception(std::make_exception_ptr(std::runtime_error(FLINE("error message"))));
        },
                             msec_, value, std::move(p));
        t.detach();
        return f;
    }

  private:
    int msec_;
};

TEST_F(FutureTest, exception) {

    auto w2 = Wait2(1000);
    auto f = w2.func(5);

    for(; f.wait_for(std::chrono::seconds(0)) != std::future_status::ready; std::this_thread::sleep_for(std::chrono::milliseconds(100))) {
        //        std::cout << "wait" << std::endl;
    }

    //    std::cout << "ready" << std::endl;
    try {
        f.get();
        //        std::cout << "success" << std::endl;
    } catch(std::exception &ex) {
        //            std::cout << "catch ex: " << ex.what() << std::endl;
    }

    //        FAIL();
}