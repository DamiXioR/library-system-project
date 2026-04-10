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
      auto micFilter = std::make_unique<DataBase::FilterMultiIndexedContainer>();
      micRawHandler = mic.get();
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

TEST_F(BookRepositoryTests, GetBookByTitle) {
  auto bookDziady = std::make_shared<Book>(bookBuilder->createDziady());
  ASSERT_TRUE(bookRepo->addBook(*bookDziady));

  auto bookHyperion = std::make_shared<Book>(bookBuilder->createHyperion());
  ASSERT_TRUE(bookRepo->addBook(*bookHyperion));

  auto bookWiedzmin = std::make_shared<Book>(bookBuilder->createWitcherTheLastWish());
  ASSERT_TRUE(bookRepo->addBook(*bookWiedzmin));

  uint16_t expectedNumOfBooksInRepository {3};
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooksInRepository);

  DataBase::FilterQuery query {
    {"book_title", {"Hyperion"}}
  };

  auto filteredBooks {bookRepo->filterBooks(query)};
  auto it {filteredBooks.find(bookHyperion)};
  ASSERT_TRUE(it == filteredBooks.end());

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
