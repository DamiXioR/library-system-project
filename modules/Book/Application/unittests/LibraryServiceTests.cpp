#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <ranges>
#include <memory>

#include "Book.hpp"
#include "BookId.hpp"
#include "MockIBookRepository.hpp"
#include "LibraryService.hpp"
#include "AddBookCommand.hpp"
#include "RemoveBookCommand.hpp"
#include "BookBuilderHelper.hpp"
#include "BookFilteringForMultiIndexedContainer.hpp"
#include "MockIUuidGenerator.hpp"

/* 
  Mocks are managed via std::unique_ptr and moved into Logger.
  Memcheck reports may include false positives due to GoogleTest/GoogleMock
  internal allocations and teardown behavior.
*/

using namespace BookHelpers;
using namespace Repository;
using namespace ::testing;

using namespace Actions;

class LibraryServiceTests : public Test {

  void SetUp() override {
      bookRepositoryMock = std::make_unique<testing::NiceMock<Repository::MockIBookRepository>>();
      bookRepositoryMockHandler = bookRepositoryMock.get();
      libraryService = std::make_unique<LibraryService>(std::move(bookRepositoryMock), uuidGeneratorMock);
  }

protected:
  BookBuilder bookBuilder;
  std::unique_ptr<LibraryService> libraryService;
  std::unique_ptr<NiceMock<MockIBookRepository>> bookRepositoryMock;
  NiceMock<MockIBookRepository>* bookRepositoryMockHandler;
  StrictMock<MockIUuidGenerator> uuidGeneratorMock;
};

TEST_F(LibraryServiceTests, AddBookToRepository) {
  EXPECT_CALL(*bookRepositoryMockHandler, addBook(_));

  const AddBookCommand addBookCommand {
    .m_title = "The Witcher: The Last Wish",
    .m_author = "Andrzej Sapkowski"
  };

  EXPECT_CALL(uuidGeneratorMock, generate());
  libraryService->addBook(addBookCommand);
}

TEST_F(LibraryServiceTests, FindBookInRepository) {
  EXPECT_CALL(*bookRepositoryMockHandler, filterBooks(_));

  libraryService->filterBooks(BookFilters{});

}

TEST_F(LibraryServiceTests, RemoveBookFromRepository) {
  EXPECT_CALL(*bookRepositoryMockHandler, removeBook(_));

  const BookId bookId {"123"};
  const RemoveBookCommand removeBookCommand {
    .m_bookId = bookId
  };

  libraryService->removeBook(removeBookCommand);
}

TEST_F(LibraryServiceTests, GetNumOfBooksInRepository) {
  EXPECT_CALL(*bookRepositoryMockHandler, getNumOfBooks());

  libraryService->getNumOfBooks();
}
