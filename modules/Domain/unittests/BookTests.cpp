#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Book.hpp"
#include "BookId.hpp"

class BookTests : public ::testing::Test {

};

TEST_F(BookTests, CreateBookId) {
  const std::string uuid{"123-456-789-101012"};
  BookId bookId{uuid};
  ASSERT_EQ(uuid, bookId.getBookId());
}

TEST_F(BookTests, CreateBook) {
  const std::string uuid{"123-456-789-101012"};
  const std::string title{"Dziady"};
  const std::string author{"Adam Mickiewicz"};

  BookId bookId{uuid};
  Book book{bookId, author, title};

  ASSERT_EQ(uuid, book.getBookId().getBookId());
  ASSERT_EQ(title, book.getTitle());
  ASSERT_EQ(author, book.getAuthor());
}

TEST_F(BookTests, SetBookFields) {
  const std::string uuid{"123-456-789-101012"};
  const std::string title{"Baby"};
  const std::string author{"Adam Sienkiewicz"};

  BookId bookId{uuid};
  Book book{bookId, author, title};

  ASSERT_EQ(book.getBookId().getBookId(), uuid);
  ASSERT_EQ(book.getTitle(), title);
  ASSERT_EQ(book.getAuthor(), author);

  const std::string correctedTitle{"Dziady"};
  const std::string correctedAuthor{"Adam Mickiewicz"};

  book.setAuthor(correctedAuthor);
  book.setTitle(correctedTitle);
  ASSERT_EQ(book.getTitle(), correctedTitle);
  ASSERT_EQ(book.getAuthor(), correctedAuthor);

  const uint16_t pYear = 2000;
  ASSERT_EQ(book.getPublicationYear(), std::nullopt);
  book.setPublicationYear(pYear);
  ASSERT_EQ(book.getPublicationYear(), pYear);
}
