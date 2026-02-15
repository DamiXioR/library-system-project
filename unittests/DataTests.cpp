#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Data.hpp"

class DataTest : public ::testing::Test{

};

// Test whether GTest works properly
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST_F(DataTest, VerifyDataInitialization) {
  Data& data = Data::GetInstance();
  EXPECT_STREQ(data.welcome.c_str(), "I am singleton");
}

