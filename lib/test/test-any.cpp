#include <boost/any.hpp>
#include <boost/log/trivial.hpp>
#include <gtest/gtest.h>

class AnyTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

TEST_F(AnyTest, test) {
    boost::any hoge = std::string("abc");
    ASSERT_EQ(typeid(std::string), hoge.type());
    boost::any_cast<std::string>(hoge);

    boost::any hoge2 = "abc";
    ASSERT_EQ(typeid(const char*), hoge2.type());

    boost::any_cast<const char*>(hoge2);

//    FAIL();
}
