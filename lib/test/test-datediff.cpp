#include <gtest/gtest.h>

extern "C" {
#include "../datediff.h"
}

class DateDiffTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

TEST_F(DateDiffTest, compdate) {
    ASSERT_EQ(-1, compdate(1900, 12, 30, 1900, 12, 31));
    ASSERT_EQ(-1, compdate(2000, 1, 1, 2000, 1, 2));
    ASSERT_EQ(-1, compdate(2100, 1, 31, 2100, 2, 1));
    ASSERT_EQ(-1, compdate(2000, 2, 28, 2000, 2, 29));

    ASSERT_EQ(0, compdate(1900, 1, 1, 1900, 1, 1));
    ASSERT_EQ(0, compdate(2000, 1, 1, 2000, 1, 1));
    ASSERT_EQ(0, compdate(2100, 12, 31, 2100, 12, 31));
    ASSERT_EQ(0, compdate(2000, 2, 29, 2000, 2, 29));

    ASSERT_EQ(1, compdate(1901, 1, 1, 1900, 12, 31));
    ASSERT_EQ(1, compdate(2000, 12, 31, 2000, 12, 30));
    ASSERT_EQ(1, compdate(2100, 1, 2, 2100, 1, 1));
    ASSERT_EQ(1, compdate(2000, 2, 29, 2000, 2, 28));
}

TEST_F(DateDiffTest, checkdate) {
    ASSERT_TRUE(checkdate(1900, 1, 1));
    ASSERT_TRUE(checkdate(1900, 1, 31));
    ASSERT_TRUE(checkdate(1900, 12, 31));

    ASSERT_TRUE(checkdate(2000, 2, 29));
    ASSERT_TRUE(checkdate(2100, 12, 31));

    ASSERT_FALSE(checkdate(1899, 12, 31));
    ASSERT_FALSE(checkdate(1900, 1, 0));
    ASSERT_FALSE(checkdate(1900, 0, 1));
    ASSERT_FALSE(checkdate(1900, 13, 1));
    ASSERT_FALSE(checkdate(1900, 1, 32));
    ASSERT_FALSE(checkdate(1900, 2, 29));
    ASSERT_FALSE(checkdate(2101, 1, 1));
}

TEST_F(DateDiffTest, incdate) {
    int y = 1900;
    int m = 1;
    int d = 1;
    incdate(&y, &m, &d);
    ASSERT_EQ(1900, y);
    ASSERT_EQ(1, m);
    ASSERT_EQ(2, d);

    y = 1900;
    m = 1;
    d = 31;
    incdate(&y, &m, &d);
    ASSERT_EQ(1900, y);
    ASSERT_EQ(2, m);
    ASSERT_EQ(1, d);

    y = 1900;
    m = 2;
    d = 28;
    incdate(&y, &m, &d);
    ASSERT_EQ(1900, y);
    ASSERT_EQ(3, m);
    ASSERT_EQ(1, d);

    y = 2000;
    m = 2;
    d = 28;
    incdate(&y, &m, &d);
    ASSERT_EQ(2000, y);
    ASSERT_EQ(2, m);
    ASSERT_EQ(29, d);

    y = 2000;
    m = 12;
    d = 31;
    incdate(&y, &m, &d);
    ASSERT_EQ(2001, y);
    ASSERT_EQ(1, m);
    ASSERT_EQ(1, d);

    y = 2100;
    m = 12;
    d = 30;
    incdate(&y, &m, &d);
    ASSERT_EQ(2100, y);
    ASSERT_EQ(12, m);
    ASSERT_EQ(31, d);
}

TEST_F(DateDiffTest, checkleap) {
    ASSERT_FALSE(checkleap(2019));
    ASSERT_TRUE(checkleap(2020));
    ASSERT_FALSE(checkleap(2021));
    ASSERT_FALSE(checkleap(2022));
    ASSERT_FALSE(checkleap(2023));
    ASSERT_TRUE(checkleap(2024));
    ASSERT_FALSE(checkleap(2025));

    ASSERT_FALSE(checkleap(1900));
    ASSERT_FALSE(checkleap(1901));
    ASSERT_TRUE(checkleap(2000));
    ASSERT_FALSE(checkleap(2099));
    ASSERT_FALSE(checkleap(2100));
}

TEST_F(DateDiffTest, datediff) {
    ASSERT_EQ(-1, datediff(2000, 1, 2, 2000, 1, 1));
    ASSERT_EQ(-1, datediff(2001, 1, 1, 2000, 1, 1));

    ASSERT_EQ(0, datediff(2000, 1, 1, 2000, 1, 1));
    ASSERT_EQ(1, datediff(2000, 1, 1, 2000, 1, 2));
    ASSERT_EQ(30, datediff(2000, 1, 1, 2000, 1, 31));
    ASSERT_EQ(31, datediff(2000, 1, 1, 2000, 2, 1));
    ASSERT_EQ(29, datediff(2000, 2, 1, 2000, 3, 1));
    ASSERT_EQ(28, datediff(2001, 2, 1, 2001, 3, 1));
    ASSERT_EQ(1, datediff(2000, 12, 31, 2001, 1, 1));
    ASSERT_EQ(366, datediff(2000, 1, 1, 2001, 1, 1));
    ASSERT_EQ(365, datediff(2001, 1, 1, 2002, 1, 1));

    ASSERT_EQ(73413, datediff(1900, 1, 1, 2100, 12, 31));
}
