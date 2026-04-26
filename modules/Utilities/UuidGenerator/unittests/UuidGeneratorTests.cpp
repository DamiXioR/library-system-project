#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <array>
#include <cctype>
#include <algorithm>
#include <ranges>

#include "UuidGenerator.hpp"

class UuidGeneratorTests : public ::testing::TestWithParam<int> {
protected:
  auto validateUuidV4(const std::string_view& uuid) -> bool {
      constexpr uint16_t fullUuidSizeWithDashes {36};
      if (uuid.size() != fullUuidSizeWithDashes) return false;

      const auto isHex = [](const char singleCharacter) { 
          return std::isxdigit(static_cast<unsigned char>(singleCharacter)); 
      };

      constexpr uint16_t countOfUuidGroups {5};
      constexpr std::array<uint16_t, countOfUuidGroups> uuidGroups = {8, 4, 4, 4, 12};
      size_t actualUuidCharacter = 0;

      for (size_t groupIndex {0}; groupIndex < countOfUuidGroups; ++groupIndex) {
          auto singleGroup = uuid | std::views::drop(actualUuidCharacter) | std::views::take(uuidGroups[groupIndex]);
          if (!std::ranges::all_of(singleGroup, isHex)) return false;
          actualUuidCharacter += uuidGroups[groupIndex];

          if (uint16_t lastGroupWithDash {countOfUuidGroups - 2}; groupIndex <= lastGroupWithDash) {
              if (uuid[actualUuidCharacter] != '-') return false;
              ++actualUuidCharacter;
          }
      }

      constexpr uint16_t uuidVersionIndex {14};
      char uuidHexValue = static_cast<char>(uuid[uuidVersionIndex]);
      if (uuidHexValue != '4') return false;

      constexpr uint16_t uuidVariantIndex {19};
      char variantHexValue = static_cast<char>(std::tolower(uuid[uuidVariantIndex]));
      if (variantHexValue != '8' && variantHexValue != '9' && variantHexValue != 'a' && variantHexValue != 'b') return false;

      return true;
  }

  struct BookIdFake{};
  struct LoanIdFake{};
};


TEST_P(UuidGeneratorTests, GeneratesUuidWithCorrectRfc4122V4Format) {
  const auto uuid = UuidGenerator<BookIdFake>::createUuid();
  if(uuid.has_value()){
    ASSERT_TRUE(validateUuidV4(*uuid));
  }
}

INSTANTIATE_TEST_SUITE_P(
    Repeat20Times,
    UuidGeneratorTests,
    ::testing::Range(0, 20)
);

TEST_F(UuidGeneratorTests, GeneratorWorksForDifferentTypes) {
  (void) UuidGenerator<BookIdFake>::createUuid();
  (void) UuidGenerator<BookIdFake>::createUuid();
  uint16_t expectedNumOfBookUuids {2};
  ASSERT_EQ(UuidGenerator<BookIdFake>::getUuids().size(), expectedNumOfBookUuids);

  (void) UuidGenerator<LoanIdFake>::createUuid();
  uint16_t expectedNumOfLoanUuids {1};
  ASSERT_EQ(UuidGenerator<LoanIdFake>::getUuids().size(), expectedNumOfLoanUuids);

  (void) UuidGenerator<LoanIdFake>::createUuid();
  (void) UuidGenerator<LoanIdFake>::createUuid();
  (void) UuidGenerator<LoanIdFake>::createUuid();
  expectedNumOfLoanUuids = 4;
  ASSERT_EQ(UuidGenerator<LoanIdFake>::getUuids().size(), expectedNumOfLoanUuids);

  (void) UuidGenerator<BookIdFake>::createUuid();
  expectedNumOfBookUuids = 3;
  ASSERT_EQ(UuidGenerator<BookIdFake>::getUuids().size(), expectedNumOfBookUuids);
}
