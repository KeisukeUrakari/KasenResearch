#include <cstdint>
#include <gtest/gtest.h>

#include "../iconvwrapper.h"

class IConvWrapperTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

TEST_F(IConvWrapperTest, utf162iso2022) {
    auto converter = UTF16toiso2022Converter();

    const auto utf16str = std::vector<uint16_t>({
        0x3042, // あ
        0xFF11, // １
        0xFF21  // Ａ
    });

    const auto iso2022 = converter.convert(utf16str);

    ASSERT_EQ(6, iso2022.size());

    ASSERT_EQ(uint8_t(0x24), iso2022[0]);
    ASSERT_EQ(uint8_t(0x22), iso2022[1]);

    ASSERT_EQ(uint8_t(0x23), iso2022[2]);
    ASSERT_EQ(uint8_t(0x31), iso2022[3]);

    ASSERT_EQ(uint8_t(0x23), iso2022[4]);
    ASSERT_EQ(uint8_t(0x41), iso2022[5]);
}

TEST_F(IConvWrapperTest, iso2022toutf16) {
    auto converter = iso2022toUTF16Converter();

    const auto iso2022str = std::vector<uint8_t>({0x1B, 0x24, 0x42,
                                                  0x24, 0x22,
                                                  0x23, 0x31,
                                                  0x23, 0x41});

    const auto utfstr = converter.convert(iso2022str);

    ASSERT_EQ(3, utfstr.size());

    ASSERT_EQ(uint16_t(0x3042), utfstr[0]); // あ
    ASSERT_EQ(uint16_t(0xFF11), utfstr[1]); // １
    ASSERT_EQ(uint16_t(0xFF21), utfstr[2]); // Ａ
}

TEST_F(IConvWrapperTest, utf2iso2022) {
    auto converter = UTFtoiso2022Converter();

    const auto iso2022 = converter.convert("あ１Ａ", false);

    ASSERT_EQ(9, iso2022.size());

    ASSERT_EQ(uint8_t(0x1B), iso2022[0]);
    ASSERT_EQ(uint8_t(0x24), iso2022[1]);
    ASSERT_EQ(uint8_t(0x42), iso2022[2]);

    ASSERT_EQ(uint8_t(0x24), iso2022[3]);
    ASSERT_EQ(uint8_t(0x22), iso2022[4]);

    ASSERT_EQ(uint8_t(0x23), iso2022[5]);
    ASSERT_EQ(uint8_t(0x31), iso2022[6]);

    ASSERT_EQ(uint8_t(0x23), iso2022[7]);
    ASSERT_EQ(uint8_t(0x41), iso2022[8]);
}

TEST_F(IConvWrapperTest, utf2iso2022RmEscSeq) {
    auto converter = UTFtoiso2022Converter();

    const auto iso2022 = converter.convert("あ１Ａ");

    ASSERT_EQ(6, iso2022.size());

    ASSERT_EQ(uint8_t(0x24), iso2022[0]);
    ASSERT_EQ(uint8_t(0x22), iso2022[1]);

    ASSERT_EQ(uint8_t(0x23), iso2022[2]);
    ASSERT_EQ(uint8_t(0x31), iso2022[3]);

    ASSERT_EQ(uint8_t(0x23), iso2022[4]);
    ASSERT_EQ(uint8_t(0x41), iso2022[5]);
}

TEST_F(IConvWrapperTest, iso2022toutf) {
    auto converter = iso2022toUTFConverter();

    const auto iso2022str = std::vector<uint8_t>({0x1B, 0x24, 0x42,
                                                  0x24, 0x22,
                                                  0x23, 0x31,
                                                  0x23, 0x41});

    const auto utfstr = converter.convert(iso2022str, false);
    ASSERT_EQ("あ１Ａ", utfstr);
}

TEST_F(IConvWrapperTest, iso2022toutfAddEscSeq) {
    auto converter = iso2022toUTFConverter();

    const auto iso2022str = std::vector<uint8_t>({0x24, 0x22,
                                                  0x23, 0x31,
                                                  0x23, 0x41});

    const auto utfstr = converter.convert(iso2022str);
    ASSERT_EQ("あ１Ａ", utfstr);
}

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
