#include <boost/log/trivial.hpp>
#include <gtest/gtest.h>

class IFTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

class Interface {
  public:
    virtual void func() = 0;
};

class Impl : public Interface {
  public:
    Impl() {
        //        BOOST_LOG_TRIVIAL(trace) << "Impl const";
    }

    ~Impl() {
        //        BOOST_LOG_TRIVIAL(trace) << "Impl dest";
    }

    void func() override {
        //        BOOST_LOG_TRIVIAL(trace) << "func";
    }
};

TEST_F(IFTest, test) {
    auto pi = std::shared_ptr<Interface>(nullptr);
    {
        auto ptr = std::make_shared<Impl>();
        {
            auto ptr2 = ptr;
        }
        pi = ptr;
    }
//    BOOST_LOG_TRIVIAL(trace) << "dbg";
}