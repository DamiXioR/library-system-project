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
using namespace Repository;

class LibraryServiceTests : public ::testing::Test {
protected:
  static std::unique_ptr<BookBuilder> bookBuilder;
  std::unique_ptr<LibraryService> libraryService;

  static void SetUpTestSuite() {
      bookBuilder = std::make_unique<BookBuilder>();
  }

  static void TearDownTestSuite() {
      bookBuilder.reset();
  }

  void SetUp() override {
      auto bookRepo = std::make_unique<BookRepository>(
        std::make_unique<DataBase::MultiIndexedContainer>(), 
        std::make_unique<DataBase::FilterMultiIndexedContainer>()
      );
      libraryService = std::make_unique<LibraryService>(std::move(bookRepo));
  }

  auto addBooksToRepository(const std::vector<Book>& books){
      std::ranges::for_each(books, [libraryServiceRaw = libraryService.get()](const auto& book){
        libraryServiceRaw->addBook(book);
      });
  }

  void TearDown() override {
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

// TEST_F(LibraryServiceTests, FindBookInRepository) {
//   const Book wiedzminBook = bookBuilder->createWitcherTheLastWish();
//   std::vector<Book> moreWiedzminBooks {
//     wiedzminBook,
//     bookBuilder->createWitcherTheLastWish(),
//     bookBuilder->createWitcherSwordOfDestiny(),
//     bookBuilder->createWitcherSwordOfDestiny(),
//   };

//   addBooksToRepository(moreWiedzminBooks);

//   uint16_t expectedNumOfBooksInRepository = 4;
//   ASSERT_EQ(libraryService->getNumOfBooks(), expectedNumOfBooksInRepository);

//   const auto& wiedzminBookAuthor = wiedzminBook.getAuthor();
//   const auto found {libraryService->findBookByAuthor(wiedzminBookAuthor)};

//   ASSERT_FALSE(found.empty());

//   uint16_t expectedFoundNumOfBooks = 4;
//   ASSERT_EQ(found.size(), expectedFoundNumOfBooks);
//   ASSERT_EQ(found.front().get().getAuthor(), wiedzminBookAuthor);

// }

TEST_F(LibraryServiceTests, RemoveBookFromRepository) {
  const auto books = bookBuilder->createBooks();

  addBooksToRepository(books);

  const uint16_t expectedNumOfBooks {8};
  ASSERT_EQ(libraryService->getNumOfBooks(), expectedNumOfBooks);
}