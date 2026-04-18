#ifndef BOOK_ID_HPP
#define BOOK_ID_HPP

#include <string>

class BookId {
public:
    explicit BookId(std::string bookId) : m_bookId(bookId) {}
    BookId(const BookId& other) : m_bookId(other.m_bookId) {}
    BookId(BookId&& other) noexcept : m_bookId(std::move(other.m_bookId)) {}

    BookId& operator=(const BookId& other) {
        m_bookId = other.m_bookId;
        return *this;
    }

    BookId& operator=(BookId&& other) noexcept {
        m_bookId = std::move(other.m_bookId);
        return *this;
    }

    bool operator==(const BookId& other) const {
        return m_bookId == other.m_bookId;
    }

    auto getBookId() const noexcept -> const std::string& {
        return m_bookId;
    }

    struct BookIdHash {
        std::size_t operator()(const BookId& bookId) const noexcept {
            return std::hash<std::string>{}(bookId.m_bookId);
        }
    };
private:
    std::string m_bookId{};
};

#endif // BOOK_ID_HPP
