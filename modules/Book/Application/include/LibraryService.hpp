#ifndef LIBRARY_SERVICE_HPP
#define LIBRARY_SERVICE_HPP

#include <memory>
#include <vector>
#include <optional>
#include <functional>

#include "IBookRepository.hpp"
#include "BookFilters.hpp"
#include "FilterResults.hpp"

class LibraryService {
public:
    explicit LibraryService(std::unique_ptr<Repository::IBookRepository> bookRepo) noexcept
        : m_BookRepo(std::move(bookRepo)) {}

    auto addBook(const Book& book) noexcept -> bool {
        return m_BookRepo->addBook(book);
    }

    auto getNumOfBooks() const noexcept -> size_t {
        return m_BookRepo->getNumOfBooks();
    }

    auto filterBooks(BookFilters bookFilters) -> FilterResults {
        return m_BookRepo->filterBooks(std::move(bookFilters));
    }


private:
    std::unique_ptr<Repository::IBookRepository> m_BookRepo;
};

#endif // LIBRARY_SERVICE_HPP
