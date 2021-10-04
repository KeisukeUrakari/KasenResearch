#include <gtest/gtest.h>

#include "../iconvwrapper.h"

class IConvWrapperTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

TEST_F(IConvWrapperTest, utf2cp932) {
    constexpr char from[] = "UTF-8";
    constexpr char to[] = "CP932";

    auto iw = IConvWrppaer(to, from);

    char srcBuf[] = "あ１Ａ";
    char dstBuf[256];
    std::size_t convSize = 0;

    const auto err = iw.conv(srcBuf, dstBuf, sizeof(dstBuf), &convSize);

    ASSERT_NE((std::size_t)-1, err);
    ASSERT_EQ(6, convSize);
    ASSERT_EQ(char(0x82), dstBuf[0]);
    ASSERT_EQ(char(0xA0), dstBuf[1]);
    ASSERT_EQ(char(0x82), dstBuf[2]);
    ASSERT_EQ(char(0x50), dstBuf[3]);
    ASSERT_EQ(char(0x82), dstBuf[4]);
    ASSERT_EQ(char(0x60), dstBuf[5]);
}

TEST_F(IConvWrapperTest, cp9322utf8) {
#if 0
    constexpr char from[] = "CP932";
    constexpr char to[] = "UTF-8";

    auto iw = IConvWrppaer(to, from);

    char srcBuf[] = {char(0x82), char(0xA0), char(0x82), char(0x50), char(0x82), char(0x60)};

    char dstBuf[256];
    std::size_t convSize = 0;

    const auto err = iw.conv(srcBuf, dstBuf, sizeof(dstBuf), &convSize);

    ASSERT_NE((std::size_t)-1, err);
    ASSERT_STREQ("あ１Ａ", dstBuf);
#endif
}
