#include <gtest/gtest.h>

TEST(Sample, Addition)
{
    EXPECT_EQ(1 + 1, 2);
    std::cout << "AllIsWell\n";
}

TEST(Sample, StringCompare)
{
    EXPECT_STREQ("app", "app");
    std::cout << "AllIsWell\n";
}
