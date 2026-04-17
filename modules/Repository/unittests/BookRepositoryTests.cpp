#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Book.hpp"
#include "BookId.hpp"
#include "BookRepository.hpp"
#include "BookBuilderHelper.hpp"
#include "BookFiltersCreator.hpp"

using namespace BookHelpers;
using namespace Repository;
using namespace DataBase;

class BookRepositoryTests : public ::testing::Test {
protected:
  static std::unique_ptr<BookBuilder> bookBuilder;
  std::unique_ptr<IBookRepository> bookRepo;
  DataBase::MultiIndexedContainer* micRawHandler;
  DataBase::BookFilteringForMIC* micFilterRawHandler;
  std::unique_ptr<DataBase::BookFiltersCreator> filterCreator;

  static void SetUpTestSuite() {
      bookBuilder = std::make_unique<BookBuilder>();
  }

  static void TearDownTestSuite() {
      bookBuilder.reset();
  }

  void SetUp() override {
      auto mic = std::make_unique<DataBase::MultiIndexedContainer>();
      micRawHandler = mic.get();
      auto micFilter = std::make_unique<DataBase::BookFilteringForMIC>(micRawHandler);
      micFilterRawHandler = micFilter.get();
      bookRepo = std::make_unique<BookRepository>(std::move(mic), std::move(micFilter));

      filterCreator = std::make_unique<DataBase::BookFiltersCreator>();
  }

  void TearDown() override {
      micRawHandler = nullptr;
      bookRepo.reset();
  }
};
std::unique_ptr<BookBuilder> BookRepositoryTests::bookBuilder = nullptr;

TEST_F(BookRepositoryTests, AddBook) {
  Book bookDziady {bookBuilder->createDziady()};
  ASSERT_TRUE(bookRepo->addBook(bookDziady));

  uint16_t expectedNumOfBooks {1};
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooks);
  
  Book bookHyperion {bookBuilder->createHobbit()};
  ASSERT_TRUE(bookRepo->addBook(bookHyperion));

  Book bookWiedzmin {bookBuilder->createWitcherTheLastWish()};
  ASSERT_TRUE(bookRepo->addBook(bookWiedzmin));

  expectedNumOfBooks = 3;
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooks);
}

TEST_F(BookRepositoryTests, GetRequestedBooks) {
  auto bookDziady = std::make_shared<Book>(bookBuilder->createDziady());
  ASSERT_TRUE(bookRepo->addBook(*bookDziady));

  auto bookHyperion = std::make_shared<Book>(bookBuilder->createHyperion());
  ASSERT_TRUE(bookRepo->addBook(*bookHyperion));

  auto bookSolaris = std::make_shared<Book>(bookBuilder->createSolaris());
  ASSERT_TRUE(bookRepo->addBook(*bookSolaris));

  auto books {bookBuilder->createWitcherBooks()};

  for (const auto& bookPtr : books) {
    ASSERT_TRUE(bookRepo->addBook(*bookPtr));
  }

  uint16_t expectedNumOfBooksInRepository {11};
  ASSERT_EQ(bookRepo->getNumOfBooks(), expectedNumOfBooksInRepository);

  {
    BookFilters bf;
    filterCreator->withTitleFilter(bf, {"Hyperion"});

    auto filteredBooks {bookRepo->filterBooks(bf)};

    uint16_t expectedNumOfFilteredBooks {1};
    ASSERT_EQ(filteredBooks.size(), expectedNumOfFilteredBooks);
    ASSERT_TRUE(filteredBooks.contains(bookHyperion));
  }

  {
  /*
    All Witcher Books
  */

    BookFilters bf;
    filterCreator->withAuthorFilter(bf, {"Andrzej Sapkowski"});

    auto filteredBooks2 {bookRepo->filterBooks(bf)};

    uint16_t expectedNumOfFilteredBooks {8};
    ASSERT_EQ(filteredBooks2.size(), expectedNumOfFilteredBooks);
    for (const auto& bookPtr : books) {
      ASSERT_TRUE(filteredBooks2.contains(bookPtr));
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
    filterCreator->withPublicationYearFilter(bf, minYear, maxYear);

    auto filteredBooks3 {bookRepo->filterBooks(bf)};

    uint16_t expectedNumOfFilteredBooks {4};
    ASSERT_EQ(filteredBooks3.size(), expectedNumOfFilteredBooks);
    for (const auto& weakBook : filteredBooks3) {
      auto spBook {weakBook.lock()};
      ASSERT_TRUE(spBook);
      auto publYear {spBook->getPublicationYear()};
      ASSERT_TRUE(publYear >= minYear && publYear <= maxYear);
    }
  }
}

TEST_F(BookRepositoryTests, RemoveBookFailed) {
  Book defaultBook {bookBuilder->build()};
  auto isSuccess {bookRepo->removeBook(defaultBook.getBookId())};
  ASSERT_FALSE(isSuccess);
}

TEST_F(BookRepositoryTests, RemoveBook) {
  Book bookDziady {bookBuilder->createDziady()};
  ASSERT_TRUE(bookRepo->addBook(bookDziady));

  Book bookHyperion {bookBuilder->createHobbit()};
  ASSERT_TRUE(bookRepo->addBook(bookHyperion));

  Book bookWiedzmin {bookBuilder->createWitcherTheLastWish()};
  ASSERT_TRUE(bookRepo->addBook(bookWiedzmin));

  uint16_t expectedNumOfBooks {3};
  ASSERT_EQ(bookRepo->getNumOfBooks(), expectedNumOfBooks);

  const BookId& hyperionBookId {bookHyperion.getBookId()};

  auto it = micRawHandler->getPrimaryContainer().find(hyperionBookId);

  ASSERT_NE(it, micRawHandler->getPrimaryContainer().end());
  ASSERT_EQ(hyperionBookId, it->first);

  auto isSuccess {bookRepo->removeBook(hyperionBookId)};
  ASSERT_TRUE(isSuccess);

  --expectedNumOfBooks;
  ASSERT_EQ(bookRepo->getNumOfBooks(), expectedNumOfBooks);
}
