#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <vector>
#include <memory>

#include "MultiIndexedContainer.hpp"
#include "FilterMultiIndexedContainer.hpp"
#include "BookBuilderHelper.hpp"

using namespace BookHelpers;
using namespace DataBase;

class MultiIndexContainerTests : public ::testing::Test {
protected:
    void verifyIfPrimContainerContainsKeyAndHasExpectedSize(const BookId& id, size_t expectedSize) {
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

    BookBuilder bookBuilder;
    MultiIndexedContainer mic;    
};

/*
* ===================================
*           CONTAINER TESTS
* ===================================
*/

TEST_F(MultiIndexContainerTests, AddBookUpdatesPrimaryAndSecondaryIndexes) {
    auto book = bookBuilder.createWitcherSwordOfDestiny();

    ASSERT_TRUE(mic.addBook(book));

    uint16_t expectedNumOfMainKeysInPrimContainer {1};
    uint16_t expectedNumOfMainKeysInSecContainer {1};
    uint16_t expectedNumOfSubItemsInSecContainer {1};

    verifyIfPrimContainerContainsKeyAndHasExpectedSize(book.getBookId(), expectedNumOfMainKeysInPrimContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTitleIndex(), book.getTitle(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getAuthorIndex(), book.getAuthor(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getYearIndex(), book.getPublicationYear(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);

    std::vector<std::string> wordsInWitcherSwordOfDestiny {
        "witcher",
        "sword",
        "destiny"
    };
    auto expectedNumOfMainKeysInTitleWordsContainer {wordsInWitcherSwordOfDestiny.size()};
    uint16_t expectedNumOfBooksUnderTitleToken {1};

    for(const auto& word : wordsInWitcherSwordOfDestiny){
        verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTokensFromTitle(), word, expectedNumOfMainKeysInTitleWordsContainer, expectedNumOfBooksUnderTitleToken);
    }

    // The same book added. Key exists so only a new item is added for existed key.
    auto book2 = bookBuilder.createWitcherSwordOfDestiny();
    ASSERT_TRUE(mic.addBook(book2));

    expectedNumOfMainKeysInPrimContainer = 2;
    expectedNumOfSubItemsInSecContainer = 2;

    verifyIfPrimContainerContainsKeyAndHasExpectedSize(book2.getBookId(), expectedNumOfMainKeysInPrimContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTitleIndex(), book2.getTitle(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);

    // The other book added. Only main keys are updated in primary and secondary containers.
    auto book3 = bookBuilder.createLordOfTheRings();
    ASSERT_TRUE(mic.addBook(book3));

    expectedNumOfMainKeysInPrimContainer = 3;
    expectedNumOfMainKeysInSecContainer = 2;
    expectedNumOfSubItemsInSecContainer = 1;

    verifyIfPrimContainerContainsKeyAndHasExpectedSize(book3.getBookId(), expectedNumOfMainKeysInPrimContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTitleIndex(), book3.getTitle(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer); 
}

TEST_F(MultiIndexContainerTests, AddBookWithTheSameBookIdTwiceCausesFailure) {
    auto book = bookBuilder.createWitcherSwordOfDestiny();

    ASSERT_TRUE(mic.addBook(book));
    ASSERT_FALSE(mic.addBook(book));

    uint16_t expectedNumOfItems {1};
    ASSERT_EQ(mic.getPrimaryContainer().size(), expectedNumOfItems);
}

TEST_F(MultiIndexContainerTests, AddBookWithWeirdTitleUpdatesTitleWordsContainerCorrectly) {
    auto bookId = BookId("123");
    std::string author {"Johnny Test"};
    // Two the same words, a lot of blank signs, words smaller than 3 letters, spaces at the end
    std::string specyficBookTitle {"    Who     wants      more      spaces or or or more or a a e one?       "};
    auto book = Book(bookId, author, specyficBookTitle);

    ASSERT_TRUE(mic.addBook(book));

    std::vector<std::string> wordsInBookTitle {
        "wants",
        "spaces",
        "one"
    };

    uint16_t expectedNumOfMainKeysInPrimContainer {1};
    verifyIfPrimContainerContainsKeyAndHasExpectedSize(book.getBookId(), expectedNumOfMainKeysInPrimContainer);

    auto expectedNumOfMainKeysInTitleWordsContainer {wordsInBookTitle.size()};
    uint16_t expectedNumOfBooksUnderTitleToken {1};
    for (const auto& word : wordsInBookTitle){
        verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTokensFromTitle(), word, expectedNumOfMainKeysInTitleWordsContainer, expectedNumOfBooksUnderTitleToken);
    }

    auto bookId2 = BookId("456");
    specyficBookTitle = "    so bob     a good           a";

    // Bunch of spaces and one letter at the end
    auto book2 = Book(bookId2, author, specyficBookTitle);

    ASSERT_TRUE(mic.addBook(book2));

    std::vector<std::string> wordsInBook2Title {
        "bob",
        "good"
    };

    expectedNumOfMainKeysInPrimContainer = 2;
    verifyIfPrimContainerContainsKeyAndHasExpectedSize(book2.getBookId(), expectedNumOfMainKeysInPrimContainer);
    
    expectedNumOfMainKeysInTitleWordsContainer += wordsInBook2Title.size();
    for (const auto& word : wordsInBookTitle){
        verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTokensFromTitle(), word, expectedNumOfMainKeysInTitleWordsContainer, expectedNumOfBooksUnderTitleToken);
    }
}

TEST_F(MultiIndexContainerTests, RemoveBookUpdatesPrimaryAndSecondaryIndexes) {
    auto book = bookBuilder.createWitcherSwordOfDestiny();
    auto book2 = bookBuilder.createWitcherSwordOfDestiny();

    ASSERT_TRUE(mic.addBook(book));
    ASSERT_TRUE(mic.addBook(book2));

    uint16_t expectedNumOfMainKeysInPrimContainer {2};

    verifyIfPrimContainerContainsKeyAndHasExpectedSize(book.getBookId(), expectedNumOfMainKeysInPrimContainer);

    uint16_t expectedNumOfMainKeysInSecContainer {1};
    uint16_t expectedNumOfSubItemsInSecContainer {2};

    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTitleIndex(), book.getTitle(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getAuthorIndex(), book.getAuthor(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getYearIndex(), book.getPublicationYear(), expectedNumOfMainKeysInSecContainer, expectedNumOfSubItemsInSecContainer);

    auto expectedNumOfMainKeysInTitleWordsContainer {3};
    uint16_t expectedNumOfBooksUnderTitleToken {2};

    std::string witcherWordFromTitle {"witcher"};
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTokensFromTitle(), witcherWordFromTitle, expectedNumOfMainKeysInTitleWordsContainer, expectedNumOfBooksUnderTitleToken);

    ASSERT_TRUE(mic.removeBook(book.getBookId()));

    expectedNumOfMainKeysInPrimContainer = 1;
    ASSERT_EQ(mic.getPrimaryContainer().size(), expectedNumOfMainKeysInPrimContainer);

    expectedNumOfSubItemsInSecContainer = 1;
    ASSERT_EQ(mic.getTitleIndex().size(), expectedNumOfMainKeysInSecContainer);
    ASSERT_EQ(mic.getTitleIndex().at(book.getTitle()).size(), expectedNumOfSubItemsInSecContainer);
    
    ASSERT_EQ(mic.getAuthorIndex().size(), expectedNumOfMainKeysInSecContainer);
    ASSERT_EQ(mic.getAuthorIndex().at(book.getAuthor()).size(), expectedNumOfSubItemsInSecContainer);

    ASSERT_EQ(mic.getYearIndex().size(), expectedNumOfMainKeysInSecContainer);
    ASSERT_EQ(mic.getYearIndex().at(book.getPublicationYear()).size(), expectedNumOfSubItemsInSecContainer);

    expectedNumOfBooksUnderTitleToken = 1;
    verifyIfSecContainerContainsKeyAndHasExpectedSizes(mic.getTokensFromTitle(), witcherWordFromTitle, expectedNumOfMainKeysInTitleWordsContainer, expectedNumOfBooksUnderTitleToken);
}

class FilterMultiIndexContainerTests : public MultiIndexContainerTests {
protected:
    void addBooksToMultiIndexContainer(const std::vector<Book>& books){
        for(const auto& book : books){
            ASSERT_TRUE(mic.addBook(book));
        }
    }

    FilterMultiIndexedContainer filter;
};

/*
* ===================================
*           FILTER TESTS
* ===================================
*/

TEST_F(FilterMultiIndexContainerTests, FilterStartReplacesPreviousMicWithNewOne) {
    std::vector<Book> witcherBooks {
        bookBuilder.createWitcherSwordOfDestiny(),
        bookBuilder.createWitcherTheLastWish()
    };
    addBooksToMultiIndexContainer(witcherBooks);
    
    uint16_t expectedNumOfAddedBooks {2};
    ASSERT_EQ(mic.getPrimaryContainer().size(), expectedNumOfAddedBooks);

    std::initializer_list<std::string> titles {
        "The Witcher: Sword of Destiny",
        "The Witcher: The Last Wish",
        "Not existed title"
    };

    const auto foundBooks = filter.filterStart(&mic).withTitleFilter(titles).applyFilters();
    uint16_t expecedNumOfFoundBooks {2};
    ASSERT_EQ(foundBooks.size(), expecedNumOfFoundBooks);

    MultiIndexedContainer newMic;
    const auto foundBooksInNewMic = filter.filterStart(&newMic).withTitleFilter(titles).applyFilters();
    expecedNumOfFoundBooks = 0;
    ASSERT_EQ(foundBooksInNewMic.size(), expecedNumOfFoundBooks);
}

TEST_F(FilterMultiIndexContainerTests, FilterBooksWithRequestedTitleList) {
    addBooksToMultiIndexContainer(bookBuilder.createBooks());
    std::vector<Book> moreWitcherBooks {
        bookBuilder.createWitcherSwordOfDestiny(),
        bookBuilder.createWitcherSwordOfDestiny(),
        bookBuilder.createWitcherSwordOfDestiny()
    };
    addBooksToMultiIndexContainer(moreWitcherBooks);
    
    uint16_t expectedNumOfAddedBooks {11};
    ASSERT_EQ(mic.getPrimaryContainer().size(), expectedNumOfAddedBooks);

    std::initializer_list<std::string> titles {
        "The Witcher: Sword of Destiny",
        "1984",
        "Not existed title"
    };

    const auto foundBooks = filter.filterStart(&mic).withTitleFilter(titles).applyFilters();

    uint16_t expecedNumOfFoundBooks {4};
    ASSERT_EQ(foundBooks.size(), expecedNumOfFoundBooks);
}

TEST_F(FilterMultiIndexContainerTests, FilterBooksWithRequestedAuthorList) {
    std::vector<Book> books {bookBuilder.createBooks()};
    addBooksToMultiIndexContainer(books);
    std::vector<Book> moreBooks {
        bookBuilder.createWitcherSwordOfDestiny(),
        bookBuilder.createWitcherTheLastWish(),
        bookBuilder.createWitcherLadyOfTheLake(),
        bookBuilder.createWitcherBloodOfElves(),
        bookBuilder.createQuoVadis(),
        bookBuilder.createTheDeluge(),
        bookBuilder.createWithFireAndSword()
    };
    addBooksToMultiIndexContainer(moreBooks);
    
    std::size_t expectedNumOfAddedBooks {books.size() + moreBooks.size()};
    ASSERT_EQ(mic.getPrimaryContainer().size(), expectedNumOfAddedBooks);

    std::initializer_list<std::string> authors {
        "Andrzej Sapkowski",
        "Henryk Sienkiewicz",
        "Not existed author"
    };

    const auto foundBooks = filter.filterStart(&mic).withAuthorFilter(authors).applyFilters();

    uint16_t expecedNumOfFoundBooks {7};
    ASSERT_EQ(foundBooks.size(), expecedNumOfFoundBooks);
}

TEST_F(FilterMultiIndexContainerTests, FilterBooksWithRequestedPublicationYearRange) {
    std::vector<Book> witcherBooks {
        bookBuilder.createWitcherSwordOfDestiny(),
        bookBuilder.createWitcherTheLastWish(),
        bookBuilder.createWitcherLadyOfTheLake(),
        bookBuilder.createWitcherBloodOfElves(),
        bookBuilder.createWitcherBaptismOfFire(),
        bookBuilder.createWitcherTimeOfContempt(),
        bookBuilder.createWitcherTowerOfSwallows()
    };
    addBooksToMultiIndexContainer(witcherBooks);
    
    std::size_t expectedNumOfAddedBooks {witcherBooks.size()};
    ASSERT_EQ(mic.getPrimaryContainer().size(), expectedNumOfAddedBooks);

    uint16_t minYear {1989};
    uint16_t maxYear {1996};

    const auto foundBooks = filter.filterStart(&mic).withPublicationYearFilter(minYear, maxYear).applyFilters();

    uint16_t expecedNumOfFoundBooks {5};
    ASSERT_EQ(foundBooks.size(), expecedNumOfFoundBooks);
}

TEST_F(FilterMultiIndexContainerTests, FilterBooksWithMultipleFiltres) {
    std::vector<Book> books {
        bookBuilder.createWitcherSwordOfDestiny(),
        bookBuilder.createWitcherTheLastWish(),
        bookBuilder.createWitcherLadyOfTheLake(),
        bookBuilder.createWitcherBloodOfElves(),
        bookBuilder.createWitcherBaptismOfFire(),
        bookBuilder.createWitcherTimeOfContempt(),
        bookBuilder.createWitcherTowerOfSwallows(),
        bookBuilder.createHyperion()
    };
    addBooksToMultiIndexContainer(books);
    
    std::size_t expectedNumOfAddedBooks {books.size()};
    ASSERT_EQ(mic.getPrimaryContainer().size(), expectedNumOfAddedBooks);

    uint16_t minYear {1989};
    uint16_t maxYear {1996};

    std::initializer_list<std::string> authors {
        "Andrzej Sapkowski"
    };

    const auto foundBooks = filter.filterStart(&mic).withPublicationYearFilter(minYear, maxYear).withAuthorFilter(authors).applyFilters();

    uint16_t expecedNumOfFoundBooks {5};
    ASSERT_EQ(foundBooks.size(), expecedNumOfFoundBooks);
}