#ifndef IBOOK_REPOSITORY_HPP
#define IBOOK_REPOSITORY_HPP

#include <unordered_map>
#include <optional>
#include <functional>
#include <memory>
#include <vector>

#include "Book.hpp"
#include "BookId.hpp"

class IBookRepository {
public:
    virtual ~IBookRepository() = default; 
    virtual auto getBookById(const BookId& id) const noexcept -> std::optional<std::reference_wrapper<const Book>> = 0;
    virtual auto getBookByTitle(const std::string& title) const noexcept -> std::vector<std::reference_wrapper<const Book>> = 0;
    virtual auto getBookByAuthor(const std::string& author) const noexcept -> std::vector<std::reference_wrapper<const Book>> = 0;
    virtual auto addBook(const Book& book) noexcept -> bool = 0;
    virtual auto removeBook(const BookId& bookId) noexcept -> bool = 0;
    virtual auto getNumOfBooks() const noexcept -> size_t = 0;
protected:
    std::unordered_map<BookId, Book, BookId::BookIdHash> m_books;
};

#endif // IBOOK_REPOSITORY_HPP
