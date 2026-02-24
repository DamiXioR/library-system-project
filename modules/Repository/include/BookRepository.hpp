#ifndef BOOK_REPOSITORY_HPP
#define BOOK_REPOSITORY_HPP

#include <unordered_map>
#include <optional>
#include <functional>
#include <memory>
#include <vector>

#include "IBookRepository.hpp"

class BookRepository : public IBookRepository {
public:
    auto getBookById(const BookId& id) const noexcept -> std::optional<std::reference_wrapper<const Book>> override;
    auto getBookByTitle(const std::string& title) const noexcept -> std::vector<std::reference_wrapper<const Book>> override;
    auto getBookByAuthor(const std::string& author) const noexcept -> std::vector<std::reference_wrapper<const Book>> override;
    auto addBook(const Book& book) noexcept -> bool override;
    auto removeBook(const BookId& bookId) noexcept -> bool override;
    auto getNumOfBooks() const noexcept -> size_t override;
};

#endif // BOOK_REPOSITORY_HPP
