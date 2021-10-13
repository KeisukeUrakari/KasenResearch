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
    //    auto ft = FreeType("/usr/share/fonts/truetype/fonts-japanese-mincho.ttf");
    auto ft = FreeType("/usr/share/fonts/google-noto-cjk/NotoSansCJK-Bold.ttc");

    //    const auto &dot = ft.draw("１"); // 12 4
    //    const auto &dot = ft.draw("２");    // 11 8
    const auto sz = int(32);
    const auto &ftbitmaps = ft.draw("１あｗΜｇ", sz);
    //    const auto &ftbitmaps = ft.draw("１あｗΜ", sz);

    for(const auto &fb : ftbitmaps) {
        std::cout << "h: " << fb.height << " w: " << fb.width << std::endl;
        std::cout << "ax: " << fb.advanceX << " ay : " << fb.advanceY << std::endl;
        std::cout << "left: " << fb.left << " top : " << fb.top << std::endl;
        std::cout << "decender: " << fb.decender <<  " desc: " << fb.desc << " asc: " << fb.asc <<  std::endl;

        print(fb.rows);
    }

    auto ascMax = int(0);
    auto descMax = int(0);
    for(const auto &fb : ftbitmaps) {
        ascMax = (ascMax < fb.asc) ? fb.asc : ascMax;
        descMax = (descMax < fb.desc) ? fb.desc : descMax;
    }

    auto width = int(0);
    for(const auto &fb : ftbitmaps) {
        width += fb.advanceX;
    }
    const auto height = ascMax + descMax;

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
                //                const auto yy = top + y + (height - fb.top) ;// + fb.decender;
                const auto yy = top + y + (height - fb.top) - descMax;
                const auto xx = left + x + fb.left;
                if(yy >= 0 && yy < height && xx >= 0 && xx < width) {
                    out[yy][xx] = color;
                }
            }
        }
        left += fb.advanceX;
    }

    const auto white = png::basic_rgba_pixel<uint8_t>(uint8_t(255), uint8_t(255), uint8_t(255));
    const auto blue = png::basic_rgba_pixel<uint8_t>(uint8_t(0), uint8_t(0), uint8_t(255));
    for(int x = 0; x < width; ++x) {
        out[ascMax][x] = white;
        //        out[sz][x] = blue;
    }

    out.write("/tmp/output.png");
    FAIL();
}