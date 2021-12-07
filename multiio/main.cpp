#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/trivial.hpp>
#include <functional>
#include <future>
#include <thread>

class SC {
  public:
    SC() : cnt_(0), promise_(nullptr), io_context_(boost::asio::io_context()),
           timer_(io_context_, boost::asio::chrono::seconds(1)),
           work_(io_context_) {
    }

    void post() {
        io_context_.post([]() {
            BOOST_LOG_TRIVIAL(trace) << "SC post";
        });
    }

    std::future<int> block() {
        auto p = std::make_shared<std::promise<int>>();
        auto f = p->get_future();

        io_context_.post([this, p]() {
            privateFunc(p);
        });

        return f;
    }

    void tick(int elapse) {
        io_context_.post([this, elapse]() {
            tick_(elapse);
        });
    }

    void run() {
        //        timer_.async_wait(boost::bind(&SC::onTimer, this));
        auto t = std::thread([&]() {
            io_context_.run();
        });
        t.detach();
    }

  private:
    void tick_(int elapse) {
        BOOST_LOG_TRIVIAL(trace) << "SC tick" << elapse;

        if(promise_ != nullptr) {
            cnt_ += 1;
            if(cnt_ >= 4) {
                cnt_ = 0;
                promise_->set_value(123);
                promise_.reset();
            }
        }
    }

    void onTimer() {
        BOOST_LOG_TRIVIAL(trace) << "SC ontimer";
        timer_.expires_at(timer_.expiry() + boost::asio::chrono::milliseconds(500));
        timer_.async_wait(boost::bind(&SC::onTimer, this));
    }

    void privateFunc(std::shared_ptr<std::promise<int>> promise) {
        BOOST_LOG_TRIVIAL(trace) << "SC private func";
        promise_ = promise;

//        promise->set_value(123);
    }

    int cnt_;
    std::shared_ptr<std::promise<int>> promise_;
    boost::asio::io_context io_context_;
    boost::asio::steady_timer timer_;
    boost::asio::io_context::work work_;
};

class Caller {
public:

private:

};

class App {
  public:
    App() : io_context_(boost::asio::io_context()),
            timer_(io_context_, boost::asio::chrono::seconds(1)) {
        sc_.push_back(std::make_shared<SC>());
    }

    void run() {
        timer_.async_wait(boost::bind(&App::onTimer, this));
        for(auto &sc : sc_) {
            sc->run();
        }

        auto t = std::thread([this](){
            for(;;) {
                for(auto &sc : sc_) {
                    const auto v = sc->block().get();
                    BOOST_LOG_TRIVIAL(trace) << "promise: " << v;
                }
            }
        });
        t.detach();

        io_context_.run();
    }

  private:
    void onTimer() {
        BOOST_LOG_TRIVIAL(trace) << "App ontimer";
        timer_.expires_at(timer_.expiry() + boost::asio::chrono::milliseconds(500));
        timer_.async_wait(boost::bind(&App::onTimer, this));
        //        sc_[0]->post();
        for(auto &sc : sc_) {
            sc->tick(500);
//            std::cout << sc->block().get() << std::endl;
        }
    }

    boost::asio::io_context io_context_;
    boost::asio::steady_timer timer_;

    std::vector<std::shared_ptr<SC>> sc_;
};

int main(int argc, char *argv[]) {
    App app;

    app.run();

    return 0;
}