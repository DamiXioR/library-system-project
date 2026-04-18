#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <vector>
#include <memory>

#include "BookBuilderHelper.hpp"
#include "MultiIndexedContainer.hpp"
#include "BookFilteringForMultiIndexedContainer.hpp"

using namespace BookHelpers;
using namespace DataBase;
using namespace BookFiltering;

class BookFilteringForMultiIndexedContainerTests : public ::testing::Test {
    
    void SetUp() override {
        filter = std::make_unique<BookFilteringForMultiIndexedContainer>(&mic);
    }

protected:
    void addBooksToMultiIndexContainer(const std::vector<Book>& books){
        for(const auto& book : books){
            ASSERT_TRUE(mic.addBook(book));
        }
    }

    std::unique_ptr<BookFilteringForMultiIndexedContainer> filter;
    BookBuilder bookBuilder;
    MultiIndexedContainer mic;
};

/*
* ===================================
*           FILTER TESTS
* ===================================
*/

TEST_F(BookFilteringForMultiIndexedContainerTests, FilterStartReplacesPreviousMicWithNewOne) {
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

    BookFilters bf;
    bf.titleFilter = {true, std::vector<std::string>(titles)};

    const auto foundBooks = filter->filterService(bf);
    uint16_t expecedNumOfFoundBooks {2};
    ASSERT_EQ(foundBooks.m_books.size(), expecedNumOfFoundBooks);

    MultiIndexedContainer newMic;
    filter->swapMic(&newMic);

    const auto foundBooksInNewMic = filter->filterService(bf);
    expecedNumOfFoundBooks = 0;
    ASSERT_EQ(foundBooksInNewMic.m_books.size(), expecedNumOfFoundBooks);
}

TEST_F(BookFilteringForMultiIndexedContainerTests, FilterBooksWithRequestedTitleList) {
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

    BookFilters bf;
    bf.titleFilter = {true, std::vector<std::string>(titles)};

    const auto foundBooks = filter->filterService(bf);

    uint16_t expecedNumOfFoundBooks {4};
    ASSERT_EQ(foundBooks.m_books.size(), expecedNumOfFoundBooks);
}

TEST_F(BookFilteringForMultiIndexedContainerTests, FilterBooksWithRequestedAuthorList) {
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

    BookFilters bf;
    bf.authorFilter = {true, std::vector<std::string>(authors)};

    const auto foundBooks = filter->filterService(bf);

    uint16_t expecedNumOfFoundBooks {7};
    ASSERT_EQ(foundBooks.m_books.size(), expecedNumOfFoundBooks);
}

TEST_F(BookFilteringForMultiIndexedContainerTests, FilterBooksWithRequestedPublicationYearRange) {
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

    BookFilters bf;
    bf.pYearFilter = {true, {minYear, maxYear}};

    const auto foundBooks = filter->filterService(bf);

    uint16_t expecedNumOfFoundBooks {5};
    ASSERT_EQ(foundBooks.m_books.size(), expecedNumOfFoundBooks);
}


TEST_F(BookFilteringForMultiIndexedContainerTests, FilterBooksWithFilter) {
    auto books {bookBuilder.createBooks()};

    addBooksToMultiIndexContainer(books);

    auto witcherBooks {bookBuilder.createWitcherBooks()};
    addBooksToMultiIndexContainer(witcherBooks);

    std::size_t expectedNumOfBooks {books.size() + witcherBooks.size()};
    ASSERT_EQ(mic.getPrimaryContainer().size(), expectedNumOfBooks);

    {
        /*
            Only one Hyperion in books
        */
        BookFilters bf;
        bf.titleFilter = {true, {"Hyperion"}};

        auto filterResults {filter->filterService(bf)};
        auto& filteredBooks {filterResults.m_books};

        uint16_t expectedNumOfFilteredBooks {1};
        ASSERT_EQ(filteredBooks.size(), expectedNumOfFilteredBooks);

        auto spBook = filteredBooks.begin()->lock();
        ASSERT_TRUE(spBook);
        ASSERT_EQ(spBook->getTitle(), "Hyperion");
    }

    {
        /*
            All Witcher books
        */

        BookFilters bf;
        bf.authorFilter = {true, {"Andrzej Sapkowski"}};

        auto filterResults {filter->filterService(bf)};
        auto& filteredBooks {filterResults.m_books};

        uint16_t expectedNumOfFilteredBooks {8};
        ASSERT_EQ(filteredBooks.size(), expectedNumOfFilteredBooks);

        for (const auto& book : witcherBooks) {
            ASSERT_TRUE(filteredBooks.contains(std::make_shared<Book>(book)));
        }
    }

    {
        /*
            1993 – The Witcher: The Last Wish
            1994 – The Witcher: Blood of Elves
            1995 – The Witcher: Time of Contempt
            1996 – The Witcher: Baptism of Fire
        */

        uint16_t minYear {1993};
        uint16_t maxYear {1996};

        BookFilters bf;
        bf.pYearFilter = {true, {minYear, maxYear}};

        auto filterResults {filter->filterService(bf)};
        auto& filteredBooks {filterResults.m_books};

        uint16_t expectedNumOfFilteredBooks {4};
        ASSERT_EQ(filteredBooks.size(), expectedNumOfFilteredBooks);

        for (const auto& weakBook : filteredBooks) {
            auto spBook {weakBook.lock()};
            ASSERT_TRUE(spBook);
            auto publYear {spBook->getPublicationYear()};
            ASSERT_TRUE(publYear >= minYear && publYear <= maxYear);
        }
    }
}

TEST_F(BookFilteringForMultiIndexedContainerTests, FilterBooksWithMultipleFiltres) {
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

    BookFilters bf;
    bf.authorFilter = {true, std::vector<std::string>(authors)};
    bf.pYearFilter = {true, {minYear, maxYear}};

    const auto foundBooks = filter->filterService(bf);

    uint16_t expecedNumOfFoundBooks {5};
    ASSERT_EQ(foundBooks.m_books.size(), expecedNumOfFoundBooks);
}
