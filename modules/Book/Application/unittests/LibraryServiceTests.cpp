#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <ranges>
#include <memory>

#include "Book.hpp"
#include "BookId.hpp"
#include "MockIBookRepository.hpp"
#include "LibraryService.hpp"
#include "BookBuilderHelper.hpp"
#include "BookFilteringForMultiIndexedContainer.hpp"

using namespace BookHelpers;
using namespace Repository;
using namespace ::testing;

class LibraryServiceTests : public Test {

  void SetUp() override {
      bookRepositoryMock = std::make_unique<testing::NiceMock<Repository::MockIBookRepository>>();
      bookRepositoryMockHandler = bookRepositoryMock.get();
      libraryService = std::make_unique<LibraryService>(std::move(bookRepositoryMock));
  }

protected:
  BookBuilder bookBuilder;
  std::unique_ptr<LibraryService> libraryService;
  std::unique_ptr<NiceMock<MockIBookRepository>> bookRepositoryMock;
  NiceMock<MockIBookRepository>* bookRepositoryMockHandler;
};

TEST_F(LibraryServiceTests, AddBookToRepository) {
  EXPECT_CALL(*bookRepositoryMockHandler, addBook(::testing::_));

  const Book wiedzminBook = bookBuilder.createWitcherTheLastWish();
  libraryService->addBook(wiedzminBook);
}

TEST_F(LibraryServiceTests, FindBookInRepository) {
  EXPECT_CALL(*bookRepositoryMockHandler, filterBooks(::testing::_));

  libraryService->filterBooks(BookFilters{});

}

TEST_F(LibraryServiceTests, RemoveBookFromRepository) {
  EXPECT_CALL(*bookRepositoryMockHandler, getNumOfBooks());

  libraryService->getNumOfBooks();
}