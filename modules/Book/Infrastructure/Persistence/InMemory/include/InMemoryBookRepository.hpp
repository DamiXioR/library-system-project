#ifndef BOOK_REPOSITORY_HPP
#define BOOK_REPOSITORY_HPP

#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <functional>
#include <memory>

#include "IBookRepository.hpp"
#include "MultiIndexedContainer.hpp"
#include "BookFilteringForMultiIndexedContainer.hpp"

namespace Repository {

class InMemoryBookRepository : public IBookRepository {
public:
    InMemoryBookRepository(
        std::unique_ptr<DataBase::MultiIndexedContainer> dataContainer,
        std::unique_ptr<BookFiltering::BookFilteringForMultiIndexedContainer> dataFilter
    )
    : m_dataContainer(std::move(dataContainer))
    , m_bookFiltering(std::move(dataFilter))
    {}

    auto addBook(Book book) noexcept -> bool override;
    auto removeBook(const BookId& bookId) noexcept -> bool override;
    auto getNumOfBooks() const noexcept -> size_t override;

    auto filterBooks(BookFilters bookFilters) -> FilterResults override;

private:
    std::unique_ptr<DataBase::MultiIndexedContainer> m_dataContainer;
    std::unique_ptr<BookFiltering::BookFilteringForMultiIndexedContainer> m_bookFiltering;
};

} // Repository namespace

#endif // BOOK_REPOSITORY_HPP
