#ifndef IBOOK_REPOSITORY_HPP
#define IBOOK_REPOSITORY_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include "IDataContainer.hpp"
#include "IBookFilteringForMIC.hpp"

#include "Book.hpp"
#include "BookId.hpp"

namespace Repository {

class IBookRepository {
public:
    IBookRepository(std::unique_ptr<DataBase::IDataContainer> dataContainer, std::unique_ptr<DataBase::IBookFilteringForMIC> dataFilter) :
                    m_dataContainer(std::move(dataContainer)), 
                    m_dataFilter(std::move(dataFilter))
                    {}
    virtual ~IBookRepository() = default; 
    virtual auto filterBooks(BookFilters bookFilters) -> DataBase::FilteredBooks = 0;
    virtual auto addBook(Book book) noexcept -> bool = 0;
    virtual auto removeBook(const BookId& bookId) noexcept -> bool = 0;
    virtual auto getNumOfBooks() const noexcept -> size_t = 0;
protected:
    std::unique_ptr<DataBase::IDataContainer> m_dataContainer;
    std::unique_ptr<DataBase::IBookFilteringForMIC> m_dataFilter;
};

} // Repository namespace

#endif // IBOOK_REPOSITORY_HPP
