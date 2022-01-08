#include <gtest/gtest.h>
#include <cstdlib>

#include "../memstream.h"
#include "../png++-0.2.10/png.hpp"

class MemStreamTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(MemStreamTest, test1) {
    char buffer[1000];
    CExternalBuffer streamBuffer(buffer, sizeof(buffer));
    std::iostream stream(&streamBuffer);
    stream << "hoge";
}

#if 0
TEST_F(MemStreamTest, test2) {
    char buffer[1000];

    CExternalStream stream(buffer, sizeof(buffer));
    stream << "hoge";

    FAIL();
}
#endif

TEST_F(MemStreamTest, test3) {
    const auto sz = std::size_t(1024 * 1024);
    char *buffer = new char[sz];
    memset(buffer, 0x00, sz);
    CExternalBuffer streamBuffer(buffer, sz);
    std::iostream stream(&streamBuffer);

    png::image<png::rgba_pixel> image("../lib/test/data/pngtest8rgba.png");
    image.write_stream(stream);

    const auto csz = streamBuffer.getContentSize();

    std::ofstream ofs("/tmp/tmp2.png", std::ios::binary);
    ofs.write(buffer, csz);
    ofs.close();

    delete[] buffer;
    FAIL();
}

TEST_F(MemStreamTest, test4) {
    const auto sz = std::size_t(1024 * 1024);
    char *buffer = new char[sz];
    memset(buffer, 0x00, sz);

    std::ifstream ifs("../lib/test/data/pngtest8rgba.png", std::ios::binary);
    ifs.seekg(0, std::ios::end);
    const auto fsize = ifs.tellg();
    ifs.seekg(0);

    ifs.read((char *)buffer, fsize);
    ifs.close();

    CExternalBuffer streamBuffer(buffer, sz);
    std::iostream stream(&streamBuffer);

    png::image<png::rgba_pixel> image(stream);

    image.write("/tmp/tmp.png");

    delete[] buffer;
    FAIL();
}
