#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Book.hpp"
#include "BookId.hpp"
#include "UuidGenerator.hpp"

class BookShelfTests : public ::testing::Test {

};

TEST_F(BookShelfTests, CreateBookId) {
  const std::string uuid{"123-456-789-101012"};
  BookId bookId{uuid};
  ASSERT_EQ(uuid, bookId.getBookId());
}

TEST_F(BookShelfTests, CreateBook) {
  const std::string uuid{"123-456-789-101012"};
  const std::string title{"Dziady"};
  const std::string author{"Adam Mickiewicz"};

  BookId bookId{uuid};
  Book book{bookId, author, title};

  ASSERT_EQ(uuid, book.getBookId().getBookId());
  ASSERT_EQ(title, book.getTitle());
  ASSERT_EQ(author, book.getAuthor());
}
