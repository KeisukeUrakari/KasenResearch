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
    const int fontsz = 48;
    //auto ft = FreeType("/usr/share/fonts/truetype/fonts-japanese-gothic.ttf", fontsz);
    //auto ft = FreeType("/usr/share/fonts/truetype/fonts-japanese-mincho.ttf",fontsz);
    //auto ft = FreeType("/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",fontsz);
    auto ft = FreeType("/usr/share/fonts/opentype/noto/NotoSerifCJK-Regular.ttc", fontsz);
    //auto ft = FreeType("/usr/share/fonts/google-noto-cjk/NotoSansCJK-Bold.ttc",fontsz);
    const auto &ftbitmaps = ft.draw("１あｗΜｇ！");

    const auto height = fontsz;
    auto width = int(0);

    for(const auto &fb : ftbitmaps) {
        std::cout << "base: " << ft.baseline() << " asc: " << fb.asc << " desc: " << fb.desc << " w: " << fb.width << std::endl;
        print(fb.rows);
        width += ft.dotsize();
    }

    auto out = png::image<png::rgba_pixel>(width, height);

    for(auto x = 0; x < width; ++x) {
        const auto blue = png::basic_rgba_pixel<uint8_t>(0, 0, 255);
        out[ft.baseline()][x] = blue;
    }

    for(std::size_t i = 0; i < ftbitmaps.size(); ++i) {
        for(int y = 0; y < ft.dotsize(); ++y) {
            const auto yel = png::basic_rgba_pixel<uint8_t>(255, 255, 0);
            out[y][i * 48] = yel;
        }
    }

    auto left = int(0);

    for(const auto &fb : ftbitmaps) {
        for(decltype(fb.rows.size()) y = 0; y < fb.rows.size(); ++y) {
            const auto &row = fb.rows[y];
            for(decltype(row.size()) x = 0; x < row.size(); ++x) {
                const auto red = png::basic_rgba_pixel<uint8_t>(255, 0, 0);
                const auto black = png::basic_rgba_pixel<uint8_t>(0, 0, 0);
                const auto &color = row[x] ? red : black;
                const auto yy = ft.baseline() - fb.asc + y;
                const auto xx = left + x + (ft.dotsize() - fb.width) / 2 + (ft.dotsize() - fb.width) % 2;
                if(yy >= 0 && yy < height && xx >= 0 && xx < width) {
                    out[yy][xx] = color;
                }
            }
        }
        left += ft.dotsize();
    }

    out.write("/tmp/output.png");
}
