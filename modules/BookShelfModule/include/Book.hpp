#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>

#include "Book.hpp"
#include "BookId.hpp"

class Book {
public:
    Book(BookId bookId, std::string author, std::string title) noexcept : m_bookId(bookId), m_author(author), m_title(title) {};
    auto getAuthor() const noexcept -> const std::string& {
        return m_author;
    }
    auto getTitle() const noexcept -> const std::string&  {
        return m_title;
    }
    auto getBookId() const noexcept -> const BookId& {
        return m_bookId;
    }
private:
    const BookId m_bookId;
    const std::string m_title;
    const std::string m_author;
};

#endif // BOOK_HPP
