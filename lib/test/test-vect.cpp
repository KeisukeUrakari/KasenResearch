#include <gtest/gtest.h>

class VectTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
        for(std::size_t i = 0; i < n; i++) {
            strings1.push_back("hoge");
            strings2[i] = "hoge";
        }
    }

    virtual void TearDown() {
    }

    constexpr static std::size_t n = std::size_t(1000);
    std::vector<std::string> strings1;
    std::size_t loopNum = 1000000;
    std::string strings2[n];
};

void func1(const std::vector<std::string> &strings) {
    return;
}

void func2(const std::string strings[]) {
    return;
}

TEST_F(VectTest, test1) {
    for(std::size_t i = 0; i < loopNum; ++i) {
        func1(strings1);
    }
}

TEST_F(VectTest, test2) {
    for(std::size_t i = 0; i < loopNum; ++i) {
        func2(strings2);
    }
}
