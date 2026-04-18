#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Book.hpp"
#include "BookId.hpp"
#include "InMemoryBookRepository.hpp"
#include "BookBuilderHelper.hpp"

using namespace BookHelpers;
using namespace Repository;
using namespace DataBase;

class InMemoryBookRepositoryTests : public ::testing::Test {

  void SetUp() override {
      bookBuilder = std::make_unique<BookBuilder>();
      auto mic = std::make_unique<DataBase::MultiIndexedContainer>();
      micRawHandler = mic.get();
      auto bookFiltering {std::make_unique<BookFiltering::BookFilteringForMultiIndexedContainer>(micRawHandler)};
      bookRepo = std::make_unique<InMemoryBookRepository>(
        std::move(mic),
        std::move(bookFiltering)
      );
  }

  void TearDown() override {
      micRawHandler = nullptr;
  }

protected:
  std::unique_ptr<BookBuilder> bookBuilder;
  std::unique_ptr<IBookRepository> bookRepo;
  DataBase::MultiIndexedContainer* micRawHandler {nullptr};
};

TEST_F(InMemoryBookRepositoryTests, AddBook) {
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

TEST_F(InMemoryBookRepositoryTests, FilterBooks) {
  auto filterResults {bookRepo->filterBooks(BookFilters{})};
  ASSERT_TRUE(filterResults.m_books.empty());
}

TEST_F(InMemoryBookRepositoryTests, RemoveBookFailed) {
  Book defaultBook {bookBuilder->build()};
  auto isSuccess {bookRepo->removeBook(defaultBook.getBookId())};
  ASSERT_FALSE(isSuccess);
}

TEST_F(InMemoryBookRepositoryTests, RemoveBook) {
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
