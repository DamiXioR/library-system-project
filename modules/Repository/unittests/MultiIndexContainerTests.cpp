#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <memory>

#include "MultiIndexedContainer.hpp"
#include "BookBuilderHelper.hpp"

using namespace BookHelpers;
using namespace DataBase;

class MultiIndexContainerTests : public ::testing::Test {
protected:
  std::unique_ptr<BookBuilder> bookBuilder;

  void SetUp() {
      bookBuilder = std::make_unique<BookBuilder>();
  }

  void TearDown() {
      bookBuilder.reset();
  }
};

void verifyIfPrimContainerContainsKeyAndHasExpectedSize(const MultiIndexedContainer& mic, const BookId& id, size_t expectedSize) {
    const auto& primary = mic.getPrimaryContainer();
    ASSERT_EQ(primary.size(), expectedSize);
    ASSERT_TRUE(primary.contains(id));
    auto it = primary.find(id);
    ASSERT_NE(it, primary.end());
    ASSERT_EQ(it->first.getBookId(), id.getBookId());
}

template<typename ContainerType, typename KeyType>
void verifyIfSecContainerContainsKeyAndHasExpectedSizes(const ContainerType& index, const KeyType& key, size_t expectedCountOfKeys, size_t expectedCountOfDataForKey) {
    ASSERT_TRUE(index.contains(key));

    auto it = index.find(key);
    ASSERT_NE(it, index.end());
    ASSERT_EQ(index.size(), expectedCountOfKeys);
    ASSERT_EQ(it->second.size(), expectedCountOfDataForKey);
}

TEST_F(MultiIndexContainerTests, AddBookUpdatesPrimaryAndSecondaryIndexes) {
    MultiIndexedContainer mic;
    auto book = bookBuilder->createWiedzminMieczPrzeznaczenia();

    ASSERT_TRUE(mic.addBook(book.getBookId(), book));

    uint16_t expectedNumOfMainKeysInPrimContainer {1};
    uint16_t expectedNumOfMainKeysInSecContainer {1};
    uint16_t expectedNumOfSubItemsInSecContainer {1};

    verifyIfPrimContainerContainsKeyAndHasExpectedSize(mic, book.getBookId(), expectedNumOfMainKeysInPrimContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTitleIndex(), book.getTitle(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getAuthorIndex(), book.getAuthor(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getYearIndex(), book.getPublicationYear(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);

    // The same book added. Key exists so only a new item is added for existed key.
    auto book2 = bookBuilder->createWiedzminMieczPrzeznaczenia();
    ASSERT_TRUE(mic.addBook(book2.getBookId(), book2));

    expectedNumOfMainKeysInPrimContainer = 2;
    expectedNumOfSubItemsInSecContainer = 2;

    verifyIfPrimContainerContainsKeyAndHasExpectedSize(mic, book2.getBookId(), expectedNumOfMainKeysInPrimContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTitleIndex(), book2.getTitle(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);

    // The other book added. Only main keys are updated in primary and secondary containers.
    auto book3 = bookBuilder->createLordOfTheRings();
    ASSERT_TRUE(mic.addBook(book3.getBookId(), book3));

    expectedNumOfMainKeysInPrimContainer = 3;
    expectedNumOfMainKeysInSecContainer = 2;
    expectedNumOfSubItemsInSecContainer = 1;

    verifyIfPrimContainerContainsKeyAndHasExpectedSize(mic, book3.getBookId(), expectedNumOfMainKeysInPrimContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTitleIndex(), book3.getTitle(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);
}

TEST_F(MultiIndexContainerTests, AddBookWithTheSameBookIdTwiceCausesFailure) {

    MultiIndexedContainer mic;

    auto book = bookBuilder->createWiedzminMieczPrzeznaczenia();

    ASSERT_TRUE(mic.addBook(book.getBookId(), book));
    ASSERT_FALSE(mic.addBook(book.getBookId(), book));

    uint16_t expectedNumOfItems {1};
    ASSERT_EQ(mic.getPrimaryContainer().size(), expectedNumOfItems);
}

TEST_F(MultiIndexContainerTests, RemoveBookUpdatesPrimaryAndSecondaryIndexes) {

    MultiIndexedContainer mic;

    auto book = bookBuilder->createWiedzminMieczPrzeznaczenia();
    auto book2 = bookBuilder->createWiedzminMieczPrzeznaczenia();

    ASSERT_TRUE(mic.addBook(book.getBookId(), book));
    ASSERT_TRUE(mic.addBook(book2.getBookId(), book2));

    uint16_t expectedNumOfMainKeysInPrimContainer {2};

    ASSERT_EQ(mic.getPrimaryContainer().size(), expectedNumOfMainKeysInPrimContainer);

    uint16_t expectedNumOfMainKeysInSecContainer {1};
    uint16_t expectedNumOfMainKeysInSecContainerSet {2};

    ASSERT_EQ(mic.getTitleIndex().size(), expectedNumOfMainKeysInSecContainer);
    ASSERT_EQ(mic.getTitleIndex().at(book.getTitle()).size(), expectedNumOfMainKeysInSecContainerSet);

    ASSERT_EQ(mic.getAuthorIndex().size(), expectedNumOfMainKeysInSecContainer);
    ASSERT_EQ(mic.getAuthorIndex().at(book.getAuthor()).size(), expectedNumOfMainKeysInSecContainerSet);

    ASSERT_EQ(mic.getYearIndex().size(), expectedNumOfMainKeysInSecContainer);
    ASSERT_EQ(mic.getYearIndex().at(book.getPublicationYear()).size(), expectedNumOfMainKeysInSecContainerSet);

    ASSERT_TRUE(mic.removeBook(book.getBookId()));

    expectedNumOfMainKeysInPrimContainer = 1;
    ASSERT_EQ(mic.getPrimaryContainer().size(), expectedNumOfMainKeysInPrimContainer);

    expectedNumOfMainKeysInSecContainerSet = 1;
    ASSERT_EQ(mic.getTitleIndex().size(), expectedNumOfMainKeysInSecContainer);
    ASSERT_EQ(mic.getTitleIndex().at(book.getTitle()).size(), expectedNumOfMainKeysInSecContainerSet);
    
    ASSERT_EQ(mic.getAuthorIndex().size(), expectedNumOfMainKeysInSecContainer);
    ASSERT_EQ(mic.getAuthorIndex().at(book.getAuthor()).size(), expectedNumOfMainKeysInSecContainerSet);

    ASSERT_EQ(mic.getYearIndex().size(), expectedNumOfMainKeysInSecContainer);
    ASSERT_EQ(mic.getYearIndex().at(book.getPublicationYear()).size(), expectedNumOfMainKeysInSecContainerSet);
}
