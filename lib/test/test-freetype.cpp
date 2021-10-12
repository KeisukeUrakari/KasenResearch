#include <gtest/gtest.h>
#include <iostream>

#include "../ft.h"
#include "../png++-0.2.10/png.hpp"

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
    const auto sz = int(96);
    const auto &ftbitmaps = ft.draw("１あｗ", sz); // 14 12

    for(const auto &fb : ftbitmaps) {
        std::cout << "h: " << fb.height << " w: " << fb.width << std::endl;
        std::cout << "ax: " << fb.advanceX << " ay : " << fb.advanceY << std::endl;
        std::cout << "left: " << fb.left << " top : " << fb.top << std::endl;
        std::cout << "decender: " << fb.decender << std::endl;

        print(fb.rows);
    }

    auto width = int(0);
    auto height = sz;
    for(const auto &fb : ftbitmaps) {
        width += fb.advanceX;
    }

    auto out = png::image<png::rgba_pixel>(width, height);

    auto left = int(0);
    auto top = int(0);
    for(const auto &fb : ftbitmaps) {
        for(decltype(fb.rows.size()) y = 0; y < fb.rows.size(); ++y) {
            const auto &row = fb.rows[y];
            for(decltype(row.size()) x = 0; x < row.size(); ++x) {
                const auto red = png::basic_rgba_pixel<uint8_t>(uint8_t(255), uint8_t(0), uint8_t(0));
                const auto black = png::basic_rgba_pixel<uint8_t>(uint8_t(0), uint8_t(0), uint8_t(0));
                const auto &color = row[x] ? red : black;
                const auto yy = top + y + (height - fb.top) + fb.decender;
                const auto xx = left + x + fb.left;
                out[yy][xx] = color;
            }
        }
        left += fb.advanceX;
    }

    out.write("/tmp/output.png");
    FAIL();
}