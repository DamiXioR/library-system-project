#include <string>
#include <algorithm>
#include <memory>
#include <cstdint>

#include "Book.hpp"
#include "BookId.hpp"
#include "BookRepository.hpp"

namespace Repository {

auto BookRepository::filterBooks(DataBase::FilterQuery filterQuery) -> DataBase::FilteredBooks {
    return m_dataFilter->queryService(std::move(filterQuery));
}

auto BookRepository::addBook(Book book) noexcept -> bool {
    return m_dataContainer->addBook(book);
}

auto BookRepository::removeBook(const BookId& bookId) noexcept -> bool {
    return m_dataContainer->removeBook(bookId);
}

auto BookRepository::getNumOfBooks() const noexcept -> size_t {
    return m_dataContainer->getContainerSize();
}

} // Repository namespace
