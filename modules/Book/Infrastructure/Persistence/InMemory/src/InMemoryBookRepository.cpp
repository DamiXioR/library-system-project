#include <string>
#include <algorithm>
#include <memory>
#include <cstdint>

#include "Book.hpp"
#include "BookId.hpp"
#include "InMemoryBookRepository.hpp"

namespace Repository {

auto InMemoryBookRepository::addBook(Book book) noexcept -> bool {
    return m_dataContainer->addBook(book);
}

auto InMemoryBookRepository::removeBook(const BookId& bookId) noexcept -> bool {
    return m_dataContainer->removeBook(bookId);
}

auto InMemoryBookRepository::getNumOfBooks() const noexcept -> size_t {
    return m_dataContainer->getContainerSize();
}

auto InMemoryBookRepository::filterBooks(BookFilters bookFilters) -> FilterResults {
    return m_bookFiltering->startFiltering(std::move(bookFilters));
}

} // Repository namespace
