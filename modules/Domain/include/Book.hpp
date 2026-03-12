#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>
#include <cstdint>
#include <optional>

#include "Book.hpp"
#include "BookId.hpp"

class Book {
public:
    Book(BookId bookId, std::string author, std::string title) noexcept 
        : m_bookId(bookId), 
          m_author(author), 
          m_title(title) {};

    Book(const Book& other) noexcept = default;

    Book(Book&& other) noexcept
        : m_bookId(other.m_bookId),
          m_author(std::move(other.m_author)),
          m_title(std::move(other.m_title)),
          m_publicationYear(std::move(other.m_publicationYear)) {}

    Book& operator=(const Book& other) = delete;
    Book& operator=(Book&& other) = delete;

    auto setAuthor(const std::string& author) -> void {
        m_author = author;
    }
    auto setTitle(const std::string& title) -> void {
        m_title = title;
    }
    auto setPublicationYear(const uint16_t publicationYear) -> void {
        m_publicationYear = publicationYear;
    }

    auto getAuthor() const noexcept -> const std::string& {
        return m_author;
    }
    auto getTitle() const noexcept -> const std::string&  {
        return m_title;
    }
    auto getPublicationYear() const noexcept -> std::optional<uint16_t>  {
        return m_publicationYear;
    }
    auto getBookId() const noexcept -> const BookId& {
        return m_bookId;
    }
private:
    const BookId m_bookId;
    std::string m_title;
    std::string m_author;
    std::optional<uint16_t> m_publicationYear {};
};

#endif // BOOK_HPP
