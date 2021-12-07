#include <cassert>
#include <exception>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "../ex.h"

class ExceptionTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

static void func2() {
    auto hoge = Hoge();

    auto ptr = std::make_shared<Hoge>();

    throw std::exception();
}

TEST_F(ExceptionTest, test) {
    try {
        func2();
    } catch(std::exception &ex) {
//        std::cout << "catch: " << ex.what() << std::endl;
    }
}
