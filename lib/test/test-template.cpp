#include <boost/log/trivial.hpp>
#include <gtest/gtest.h>

class TemplateTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};


template<>
int func() {
    return 1;
}

template<>
std::string func() {
    return "";
}

#if 0

class Hoge {
  public:
    template <typename T>
    T func() const {
        BOOST_LOG_TRIVIAL(trace) << "dbg a";
        return 1;
    }

    template <>
    int func() const {
        BOOST_LOG_TRIVIAL(trace) << "dbg b";
        return 1;
    }

    template <>
    std::string func() const {
        BOOST_LOG_TRIVIAL(trace) << "dbg c";
        return "hoge";
    }
};

TEST_F(TemplateTest, test) {
    auto hoge = Hoge();
    const auto f1 = hoge.func<char>();
    const auto f2 = hoge.func<int>();
    const auto f3 = hoge.func<std::string>();
    FAIL();
}
#endif