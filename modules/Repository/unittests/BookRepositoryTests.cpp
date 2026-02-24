#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Book.hpp"
#include "BookId.hpp"
#include "BookRepository.hpp"
#include "BookBuilderHelper.hpp"

using namespace BookHelpers;

class BookRepositoryTests : public ::testing::Test {
protected:
  static std::unique_ptr<BookBuilder> bookBuilder;
  std::unique_ptr<IBookRepository> bookRepo;

  static void SetUpTestSuite() {
      bookBuilder = std::make_unique<BookBuilder>();
  }

  static void TearDownTestSuite() {
      bookBuilder.reset();
  }

  void SetUp() override {
      bookRepo = std::make_unique<BookRepository>();
  }

  void TearDown() override {
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

  Book bookWiedzmin {bookBuilder->createWiedzminOstatnieZyczenie()};
  ASSERT_TRUE(bookRepo->addBook(bookWiedzmin));

  expectedNumOfBooks = 3;
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooks);
}

TEST_F(BookRepositoryTests, GetBookByIdFailed) {
  Book defaultBook {bookBuilder->build()};
  const auto foundBook = bookRepo->getBookById(defaultBook.getBookId());
  ASSERT_FALSE(foundBook);
}

TEST_F(BookRepositoryTests, GetBookById) {
  Book bookDziady {bookBuilder->createDziady()};
  ASSERT_TRUE(bookRepo->addBook(bookDziady));

  Book bookHyperion {bookBuilder->createHobbit()};
  ASSERT_TRUE(bookRepo->addBook(bookHyperion));

  Book bookWiedzmin {bookBuilder->createWiedzminOstatnieZyczenie()};
  ASSERT_TRUE(bookRepo->addBook(bookWiedzmin));

  uint16_t expectedNumOfBooks {3};
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooks);
  
  BookId hyperionBookId {bookHyperion.getBookId()};
  const auto foundBook = bookRepo->getBookById(hyperionBookId);
  ASSERT_TRUE(foundBook);

  const Book& book = foundBook->get();
  EXPECT_EQ(book.getBookId(), hyperionBookId);
}

TEST_F(BookRepositoryTests, GetBookByTitleFailed) {
  Book defaultBook {bookBuilder->build()};
  const auto foundBook = bookRepo->getBookByTitle(defaultBook.getTitle());
  ASSERT_TRUE(foundBook.empty());
}


TEST_F(BookRepositoryTests, GetBookByTitle) {
  Book bookDziady {bookBuilder->createDziady()};
  ASSERT_TRUE(bookRepo->addBook(bookDziady));

  Book bookHyperion {bookBuilder->createHobbit()};
  ASSERT_TRUE(bookRepo->addBook(bookHyperion));

  Book bookWiedzmin {bookBuilder->createWiedzminOstatnieZyczenie()};
  ASSERT_TRUE(bookRepo->addBook(bookWiedzmin));

  uint16_t expectedNumOfBooksInRepository {3};
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooksInRepository);
  
  const std::string hyperionBookTitle {bookHyperion.getTitle()};
  const auto foundBookHyperion = bookRepo->getBookByTitle(hyperionBookTitle);
  ASSERT_FALSE(foundBookHyperion.empty());

  const Book& book = foundBookHyperion.front().get();
  EXPECT_EQ(book.getTitle(), hyperionBookTitle);

  bookRepo->addBook(bookBuilder->createWiedzminOstatnieZyczenie());
  bookRepo->addBook(bookBuilder->createWiedzminOstatnieZyczenie());
  bookRepo->addBook(bookBuilder->createWiedzminOstatnieZyczenie());

  bookRepo->addBook(bookBuilder->createWiedzminMieczPrzeznaczenia());
  bookRepo->addBook(bookBuilder->createWiedzminMieczPrzeznaczenia());

  expectedNumOfBooksInRepository = 8;
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooksInRepository);

  const std::string wiedzminBookTitle {bookWiedzmin.getTitle()};
  const auto foundBookWiedzmin = bookRepo->getBookByTitle(wiedzminBookTitle);
  ASSERT_FALSE(foundBookWiedzmin.empty());

  uint16_t expectedNumOfFoundBooks = 4;
  ASSERT_TRUE(foundBookWiedzmin.size() == expectedNumOfFoundBooks);
  for(const auto& book : foundBookWiedzmin){
      EXPECT_EQ(book.get().getTitle(), wiedzminBookTitle);
  }
}

TEST_F(BookRepositoryTests, GetBookByAuthorFailed) {
  Book defaultBook {bookBuilder->build()};
  const auto foundBook = bookRepo->getBookByAuthor(defaultBook.getTitle());
  ASSERT_TRUE(foundBook.empty());
}

TEST_F(BookRepositoryTests, GetBookByAuthor) {
  Book bookDziady {bookBuilder->createDziady()};
  ASSERT_TRUE(bookRepo->addBook(bookDziady));

  Book bookHyperion {bookBuilder->createHobbit()};
  ASSERT_TRUE(bookRepo->addBook(bookHyperion));

  Book bookWiedzmin {bookBuilder->createWiedzminOstatnieZyczenie()};
  ASSERT_TRUE(bookRepo->addBook(bookWiedzmin));

  bookRepo->addBook(bookBuilder->createWiedzminOstatnieZyczenie());
  bookRepo->addBook(bookBuilder->createWiedzminOstatnieZyczenie());
  bookRepo->addBook(bookBuilder->createWiedzminOstatnieZyczenie());

  bookRepo->addBook(bookBuilder->createWiedzminMieczPrzeznaczenia());
  bookRepo->addBook(bookBuilder->createWiedzminMieczPrzeznaczenia());

  uint16_t expectedNumOfBooksInRepository = 8;
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooksInRepository);

  const std::string wiedzminBookAuthor {bookWiedzmin.getAuthor()};
  const auto foundBookWiedzmin = bookRepo->getBookByAuthor(wiedzminBookAuthor);
  ASSERT_FALSE(foundBookWiedzmin.empty());

  uint16_t expectedNumOFoundBooks = 6;
  ASSERT_TRUE(foundBookWiedzmin.size() == expectedNumOFoundBooks);
  for(const auto& book : foundBookWiedzmin){
      EXPECT_EQ(book.get().getAuthor(), wiedzminBookAuthor);
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

  Book bookWiedzmin {bookBuilder->createWiedzminOstatnieZyczenie()};
  ASSERT_TRUE(bookRepo->addBook(bookWiedzmin));

  uint16_t expectedNumOfBooks {3};
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooks);

  BookId hyperionBookId {bookHyperion.getBookId()};

  auto isSuccess {bookRepo->removeBook(hyperionBookId)};
  ASSERT_TRUE(isSuccess);

  --expectedNumOfBooks;
  ASSERT_TRUE(bookRepo->getNumOfBooks() == expectedNumOfBooks);

  const auto foundBook = bookRepo->getBookById(hyperionBookId);
  ASSERT_FALSE(foundBook);
  ASSERT_EQ(foundBook, std::nullopt);
}
