#include <future>
#include <gtest/gtest.h>
#include <iostream>
#include <mutex>
#include <thread>

class PromiseTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

#if 0
class Worker {
  public:
    Worker() : terminate_(false), param_(-1) {
    }

    ~Worker() {
        terminate_ = true;
        t.join();
    }

    void start() {
        t = std::thread(&Worker::loop, this);
    }

    void exe(int param, std::promise<int> p) {
        auto lock = std::lock_guard<std::mutex>(mutex_);
        promise_ = std::move(p);
        param_ = param;
    }

  private:
    void loop() {
        for(; !terminate_; std::this_thread::sleep_for(std::chrono::milliseconds(50))) {
            int para = -1;
            {
                auto lock = std::lock_guard<std::mutex>(mutex_);
                para = param_;
            }
            if(para < 0) {
                continue;
            }
            std::cout << "working..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "working..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "working..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "working..." << std::endl;
            {
                auto lock = std::lock_guard<std::mutex>(mutex_);
                promise_.set_value(param_ * 2);
                param_ = -1;
                std::cout << "finish..." << std::endl;
            }
        }
    }
    bool terminate_;
    std::thread t;
    std::mutex mutex_;
    std::promise<int> promise_;
    int param_;
};

TEST_F(PromiseTest, test) {
    Worker wrk;
    wrk.start();

    auto promise = std::promise<int>();
    auto future = promise.get_future();

    wrk.exe(10, std::move(promise));

    std::cout << "other work..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "other wrok..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    const auto r = future.get();
    ASSERT_EQ(20, r);
}
#else
class Worker {
  public:
    Worker() : terminate_(false), param_(-1) {
    }

    ~Worker() {
        terminate_ = true;
        t.join();
    }

    void start() {
        t = std::thread(&Worker::loop, this);
    }

    std::future<int> exe(int param) {
        promise_ = std::promise<int>();
        auto f = promise_.get_future();

        {
            auto lock = std::lock_guard<std::mutex>(mutex_);
            param_ = param;
        }

        return f;
    }

  private:
    void loop() {
        for(; !terminate_; std::this_thread::sleep_for(std::chrono::milliseconds(50))) {
            int para = -1;
            {
                auto lock = std::lock_guard<std::mutex>(mutex_);
                para = param_;
            }
            if(para < 0) {
                continue;
            }
            std::cout << "working..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "working..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "working..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "working..." << std::endl;
            {
                auto lock = std::lock_guard<std::mutex>(mutex_);
                promise_.set_value(param_ * 2);
                param_ = -1;
                std::cout << "finish..." << std::endl;
            }
        }
    }
    bool terminate_;
    std::thread t;
    std::mutex mutex_;
    std::promise<int> promise_;
    int param_;
};

TEST_F(PromiseTest, test) {
    Worker wrk;
    wrk.start();

    auto future = wrk.exe(10);

    std::cout << "other work..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "other wrok..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    const auto r = future.get();
    ASSERT_EQ(20, r);
}

#endif