#include <gtest/gtest.h>

extern "C" {
#include <iconv.h>
}

class IConvTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

TEST_F(IConvTest, utf2cp932) {

    constexpr char from[] = "UTF-8";
    constexpr char to[] = "CP932";

    iconv_t cd = iconv_open(to, from);
    ASSERT_NE((iconv_t)-1, cd);

    char srcBuf[] = "あ１Ａ";
    char *src = srcBuf;
    std::size_t srcSize = sizeof(srcBuf);
    char dstBuf[256];
    char *dst = dstBuf;
    std::size_t dstSize = sizeof(dstBuf);

    std::size_t err = iconv(cd, &src, &srcSize, &dst, &dstSize);
    ASSERT_NE((std::size_t)-1, err);
    ASSERT_EQ(char(0x82), dstBuf[0]);
    ASSERT_EQ(char(0xA0), dstBuf[1]);
    ASSERT_EQ(char(0x82), dstBuf[2]);
    ASSERT_EQ(char(0x50), dstBuf[3]);
    ASSERT_EQ(char(0x82), dstBuf[4]);
    ASSERT_EQ(char(0x60), dstBuf[5]);

    iconv_close(cd);
}
