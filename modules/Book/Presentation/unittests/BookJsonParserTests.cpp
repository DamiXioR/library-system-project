#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "BookJsonParser.hpp"

using namespace BookParsing;

class BookJsonParserTests : public ::testing::Test {};

TEST_F(BookJsonParserTests, Start) {
  BookJsonParser parser;
}
