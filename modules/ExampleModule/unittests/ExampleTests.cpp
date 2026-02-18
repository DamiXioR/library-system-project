#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Example.hpp"

class ExampleTest : public ::testing::Test{

};

// Test whether GTest works properly
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST_F(ExampleTest, VerifyDataInitialization) {
  Example example;
  EXPECT_STREQ(example.GetExample().c_str(), "Example");
}
