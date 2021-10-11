#include <gtest/gtest.h>
#include <iostream>

#include "../ft.h"

class FreetypeTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

void print(const std::vector<std::vector<bool>> &rows) {
    for(const auto &row : rows) {
        for(const auto &dot : row) {
            std::cout << (dot ? "*" : " "); 
        }
        std::cout << std::endl;
    }
}

TEST_F(FreetypeTest, test) {
    const auto u32str = FreeType::char2u32str("あ");
    auto ft = FreeType("/usr/share/fonts/truetype/fonts-japanese-mincho.ttf");

//    const auto &dot = ft.draw("１"); // 12 4
//    const auto &dot = ft.draw("２");    // 11 8
    const auto &dot = ft.draw("あ"); // 14 12

    std::cout << "y: " << dot.size() << " x: " << dot[0].size() << std::endl;
    print(dot);

    FAIL();
}