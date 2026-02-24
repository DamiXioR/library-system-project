#ifndef LIBRARY_SERVICE_HPP
#define LIBRARY_SERVICE_HPP

#include <memory>
#include <vector>
#include <optional>
#include <functional>

#include "IBookRepository.hpp"

class LibraryService {
public:
    explicit LibraryService(std::unique_ptr<IBookRepository> bookRepo) noexcept
        : m_BookRepo(std::move(bookRepo)) {}

    auto addBook(const Book& book) noexcept -> bool {
        return m_BookRepo->addBook(book);
    }

    auto findBookById(const BookId& bookId) const noexcept -> std::optional<std::reference_wrapper<const Book>> {
        auto found = m_BookRepo->getBookById(bookId);
        return found;
    }

    auto findBookByTitle(const std::string& title) const noexcept -> std::vector<std::reference_wrapper<const Book>> {
        auto found = m_BookRepo->getBookByTitle(title);
        return found;
    }

    auto findBookByAuthor(const std::string& author) const noexcept -> std::vector<std::reference_wrapper<const Book>> {
        auto found = m_BookRepo->getBookByAuthor(author);
        return found;
    }

    auto getNumOfBooks() const noexcept -> size_t {
        return m_BookRepo->getNumOfBooks();
    }

private:
    std::unique_ptr<IBookRepository> m_BookRepo;
};

#endif // LIBRARY_SERVICE_HPP
