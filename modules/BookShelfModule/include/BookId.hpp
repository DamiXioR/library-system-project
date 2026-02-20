#ifndef BOOK_ID_HPP
#define BOOK_ID_HPP

#include <string>

class BookId {
public:
    BookId(std::string bookId) : m_bookId(bookId) {}
    auto getBookId() const noexcept -> const std::string& {
        return m_bookId;
    }
private:
    const std::string m_bookId{};
};

#endif // BOOK_ID_HPP
