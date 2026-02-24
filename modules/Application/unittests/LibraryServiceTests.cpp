#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <ranges>
#include <memory>

#include "Book.hpp"
#include "BookId.hpp"
#include "BookRepository.hpp"
#include "LibraryService.hpp"
#include "BookBuilderHelper.hpp"

using namespace BookHelpers;

class LibraryServiceTests : public ::testing::Test {
protected:
  static std::unique_ptr<BookBuilder> bookBuilder;
  std::unique_ptr<IBookRepository> bookRepo;
  std::unique_ptr<LibraryService> libraryService;

  static void SetUpTestSuite() {
      bookBuilder = std::make_unique<BookBuilder>();
  }

  static void TearDownTestSuite() {
      bookBuilder.reset();
  }

  void SetUp() override {
      bookRepo = std::make_unique<BookRepository>();
      libraryService = std::make_unique<LibraryService>(std::move(bookRepo));
  }

  auto addBooksToRepository(const std::vector<Book>& books){
      std::ranges::for_each(books, [libraryServiceRaw = libraryService.get()](const auto& book){
        libraryServiceRaw->addBook(book);
      });
  }

  void TearDown() override {
      bookRepo.reset();
      libraryService.reset();
  }
};
std::unique_ptr<BookBuilder> LibraryServiceTests::bookBuilder = nullptr;

TEST_F(LibraryServiceTests, AddBookToRepository) {
  const auto book = bookBuilder->build();
  libraryService->addBook(book);

  uint16_t expectedNumOfBooks {1};
  ASSERT_EQ(libraryService->getNumOfBooks(), expectedNumOfBooks);

  const auto books = bookBuilder->createBooks();
  ASSERT_FALSE(books.empty());
  
  addBooksToRepository(books);

  expectedNumOfBooks = 9;
  ASSERT_EQ(libraryService->getNumOfBooks(), expectedNumOfBooks);
}

TEST_F(LibraryServiceTests, FindBookByIdInRepository) {
  const auto books = bookBuilder->createBooks();

  addBooksToRepository(books);

  const uint16_t expectedNumOfBooks {8};
  ASSERT_EQ(libraryService->getNumOfBooks(), expectedNumOfBooks);

  const auto& firstBook = books.front();
  const auto found {libraryService->findBookById(firstBook.getBookId())};

  ASSERT_TRUE(found.has_value());
  ASSERT_EQ(firstBook.getTitle(), found->get().getTitle());
}

TEST_F(LibraryServiceTests, FindBookByTitleInRepository) {
  const Book wiedzminBook = bookBuilder->createWiedzminOstatnieZyczenie();
  std::vector<Book> moreWiedzminBooks {
    wiedzminBook,
    bookBuilder->createWiedzminOstatnieZyczenie(),
    bookBuilder->createWiedzminOstatnieZyczenie(),
    bookBuilder->createWiedzminOstatnieZyczenie(),
    bookBuilder->createWiedzminMieczPrzeznaczenia(),
    bookBuilder->createWiedzminMieczPrzeznaczenia()
  };

  addBooksToRepository(moreWiedzminBooks);

  uint16_t expectedNumOfBooksInRepository = 6;
  ASSERT_EQ(libraryService->getNumOfBooks(), expectedNumOfBooksInRepository);
  
  const auto& wiedzminBookTitle = wiedzminBook.getTitle();
  const auto found {libraryService->findBookByTitle(wiedzminBookTitle)};

  ASSERT_FALSE(found.empty());

  uint16_t expectedFoundNumOfBooks = 4;
  ASSERT_EQ(found.size(), expectedFoundNumOfBooks);
  ASSERT_EQ(found.front().get().getTitle(), wiedzminBookTitle);

}

TEST_F(LibraryServiceTests, FindBookByAuthorInRepository) {
  const Book wiedzminBook = bookBuilder->createWiedzminOstatnieZyczenie();
  std::vector<Book> moreWiedzminBooks {
    wiedzminBook,
    bookBuilder->createWiedzminOstatnieZyczenie(),
    bookBuilder->createWiedzminMieczPrzeznaczenia(),
    bookBuilder->createWiedzminMieczPrzeznaczenia(),
  };

  addBooksToRepository(moreWiedzminBooks);

  uint16_t expectedNumOfBooksInRepository = 4;
  ASSERT_EQ(libraryService->getNumOfBooks(), expectedNumOfBooksInRepository);

  const auto& wiedzminBookAuthor = wiedzminBook.getAuthor();
  const auto found {libraryService->findBookByAuthor(wiedzminBookAuthor)};

  ASSERT_FALSE(found.empty());

  uint16_t expectedFoundNumOfBooks = 4;
  ASSERT_EQ(found.size(), expectedFoundNumOfBooks);
  ASSERT_EQ(found.front().get().getAuthor(), wiedzminBookAuthor);

}

TEST_F(LibraryServiceTests, RemoveBookFromRepository) {
  const auto books = bookBuilder->createBooks();

  addBooksToRepository(books);

  const uint16_t expectedNumOfBooks {8};
  ASSERT_EQ(libraryService->getNumOfBooks(), expectedNumOfBooks);
}