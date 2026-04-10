#ifndef BOOK_REPOSITORY_HPP
#define BOOK_REPOSITORY_HPP

#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <functional>
#include <memory>

#include "IBookRepository.hpp"
#include "MultiIndexedContainer.hpp"
#include "FilterMultiIndexedContainer.hpp"

namespace Repository {

class BookRepository : public IBookRepository {
public:
    BookRepository(std::unique_ptr<DataBase::IDataContainer> dataContainer, std::unique_ptr<DataBase::IDataContainerFilter> dataFilter) : IBookRepository(std::move(dataContainer), std::move(dataFilter)) {}
    auto filterBooks(DataBase::FilterQuery filterQuery) -> DataBase::FilteredBooks override;
    auto addBook(Book book) noexcept -> bool override;
    auto removeBook(const BookId& bookId) noexcept -> bool override;
    auto getNumOfBooks() const noexcept -> size_t override;
};

} // Repository namespace

#endif // BOOK_REPOSITORY_HPP
