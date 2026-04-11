#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Book.hpp"
#include "BookId.hpp"
#include "BookRepository.hpp"
#include "BookBuilderHelper.hpp"

using namespace BookHelpers;
using namespace Repository;

class BookRepositoryTests : public ::testing::Test {
protected:
  static std::unique_ptr<BookBuilder> bookBuilder;
  std::unique_ptr<IBookRepository> bookRepo;
  DataBase::MultiIndexedContainer* micRawHandler;
  DataBase::FilterMultiIndexedContainer* micFilterRawHandler;

  static void SetUpTestSuite() {
      bookBuilder = std::make_unique<BookBuilder>();
  }

  static void TearDownTestSuite() {
      bookBuilder.reset();
  }

  void SetUp() override {
      auto mic = std::make_unique<DataBase::MultiIndexedContainer>();
      micRawHandler = mic.get();
      auto micFilter = std::make_unique<DataBase::FilterMultiIndexedContainer>(micRawHandler);
      micFilterRawHandler = micFilter.get();
      bookRepo = std::make_unique<BookRepository>(std::move(mic), std::move(micFilter));
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

  uint16_t expectedNumOfBooksInRepository {3};
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooksInRepository);

  DataBase::FilterQuery query {
    {DataBase::BOOK_FILTER_TYPE::Title, {"Hyperion"}}
  };

  auto filteredBooks {bookRepo->filterBooks(query)};

  uint16_t expectedNumOfFilteredBooks {1};
  ASSERT_EQ(filteredBooks.size(), expectedNumOfFilteredBooks);
  ASSERT_TRUE(filteredBooks.contains(bookHyperion));

  std::vector<std::shared_ptr<Book>> books {
      std::make_shared<Book>(bookBuilder->createWitcherSwordOfDestiny()),
      std::make_shared<Book>(bookBuilder->createWitcherTheLastWish()),
      std::make_shared<Book>(bookBuilder->createWitcherLadyOfTheLake()),
      std::make_shared<Book>(bookBuilder->createWitcherBloodOfElves()),
      std::make_shared<Book>(bookBuilder->createWitcherBaptismOfFire()),
      std::make_shared<Book>(bookBuilder->createWitcherTimeOfContempt()),
      std::make_shared<Book>(bookBuilder->createWitcherTowerOfSwallows())
  };

  for (const auto& bookPtr : books) {
    ASSERT_TRUE(bookRepo->addBook(*bookPtr));
  }

  expectedNumOfBooksInRepository = 10;
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooksInRepository);

  DataBase::FilterQuery queryAuthor {
    {DataBase::BOOK_FILTER_TYPE::Author, {"Andrzej Sapkowski"}}
  };

  auto filteredBooks2 {bookRepo->filterBooks(queryAuthor)};

  expectedNumOfFilteredBooks = 7;
  ASSERT_EQ(filteredBooks2.size(), expectedNumOfFilteredBooks);
  for (const auto& bookPtr : books) {
    ASSERT_TRUE(filteredBooks2.contains(bookPtr));
  }

  uint16_t minYear {1993};
  uint16_t maxYear {1996};
  auto publYearData {std::to_string(minYear) + std::to_string(maxYear)};
  DataBase::FilterQuery queryPublYear {
    {DataBase::BOOK_FILTER_TYPE::PublYear, {publYearData}}
  };

  auto filteredBooks3 {bookRepo->filterBooks(queryPublYear)};

  expectedNumOfFilteredBooks = 4;
  ASSERT_EQ(filteredBooks3.size(), expectedNumOfFilteredBooks);
  for (const auto& weakBook : filteredBooks3) {
    auto spBook {weakBook.lock()};
    ASSERT_TRUE(spBook);
    auto publYear {spBook->getPublicationYear()};
    ASSERT_TRUE(publYear >= minYear && publYear <= maxYear);
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
