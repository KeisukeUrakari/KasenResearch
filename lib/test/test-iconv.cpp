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

TEST_F(IConvTest, utf2jis) {
    constexpr char from[] = "UTF-8";
    constexpr char to[] = "ISO2022JP";

    iconv_t cd = iconv_open(to, from);
    ASSERT_NE((iconv_t)-1, cd);

    char srcBuf[] = "あ１Ａ";
    char *src = srcBuf;
    std::size_t srcSize = strnlen(src, sizeof(srcBuf));
    char dstBuf[256];
    char *dst = dstBuf;
    std::size_t dstSize = sizeof(dstBuf);

    std::size_t err = iconv(cd, &src, &srcSize, &dst, &dstSize);
    ASSERT_NE((std::size_t)-1, err);

    ASSERT_EQ(char(0x1B), dstBuf[0]);
    ASSERT_EQ(char(0x24), dstBuf[1]);
    ASSERT_EQ(char(0x42), dstBuf[2]);

    ASSERT_EQ(char(0x24), dstBuf[3]);
    ASSERT_EQ(char(0x22), dstBuf[4]);

    ASSERT_EQ(char(0x23), dstBuf[5]);
    ASSERT_EQ(char(0x31), dstBuf[6]);

    ASSERT_EQ(char(0x23), dstBuf[7]);
    ASSERT_EQ(char(0x41), dstBuf[8]);

    iconv_close(cd);
}

TEST_F(IConvTest, jis2utf) {
    constexpr char from[] = "ISO2022JP";
    constexpr char to[] = "UTF-8";

    iconv_t cd = iconv_open(to, from);
    ASSERT_NE((iconv_t)-1, cd);

    char srcBuf[] = {
        0x1B, 0x24, 0x42,
        0x24, 0x22,
        0x23, 0x31,
        0x23, 0x41,
        0x1B, 0x28, 0x42};
    char *src = srcBuf;
    std::size_t srcSize = sizeof(srcBuf);
    char dstBuf[256];
    char *dst = dstBuf;
    std::size_t dstSize = sizeof(dstBuf);

    std::size_t err = iconv(cd, &src, &srcSize, &dst, &dstSize);
    ASSERT_NE((std::size_t)-1, err);

    dstBuf[sizeof(dstBuf) - dstSize] = '\0';
    ASSERT_STREQ("あ１Ａ", dstBuf);

    iconv_close(cd);
}

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

TEST_F(IConvTest, cp9322utf8) {

    constexpr char from[] = "CP932";
    constexpr char to[] = "UTF-8";

    iconv_t cd = iconv_open(to, from);
    ASSERT_NE((iconv_t)-1, cd);

    char srcBuf[] = {char(0x82), char(0xA0), char(0x82), char(0x50), char(0x82), char(0x60)};

    char *src = srcBuf;
    std::size_t srcSize = sizeof(srcBuf);
    char dstBuf[256];
    char *dst = dstBuf;
    std::size_t dstSize = sizeof(dstBuf);

    const std::size_t err = iconv(cd, &src, &srcSize, &dst, &dstSize);
    const std::size_t dstLen = sizeof(dstBuf) - dstSize;
    dstBuf[dstLen] = '\0';

    ASSERT_NE((std::size_t)-1, err);
    ASSERT_STREQ("あ１Ａ", dstBuf);

    iconv_close(cd);
}
