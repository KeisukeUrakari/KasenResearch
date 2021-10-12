#include <gtest/gtest.h>

#include "../png++-0.2.10/png.hpp"

class PNGTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

TEST_F(PNGTest, test) {
    png::image<png::rgba_pixel> image("../lib/test/data/pngtest8rgba.png");

    //    image.write("output.png");

    png::image<png::rgba_pixel> out(image.get_width(), image.get_height());
    const auto buf = image.get_pixbuf();
    for(png::uint_32 y = 0; y < buf.get_height(); ++y) {
      const auto row = buf.get_row(y);
      for(std::size_t x = 0; x < row.size(); ++x) {
        out[y][x] = row[x];
      }
    }
//    image.write("/tmp/output.png");
}
