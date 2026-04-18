#ifndef IBOOK_REPOSITORY_HPP
#define IBOOK_REPOSITORY_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include "Book.hpp"
#include "BookId.hpp"
#include "FilterResults.hpp"
#include "BookFilters.hpp"

namespace Repository {

class IBookRepository {
public:
    virtual ~IBookRepository() = default;
    virtual auto addBook(Book book) noexcept -> bool = 0;
    virtual auto removeBook(const BookId& bookId) noexcept -> bool = 0;
    virtual auto getNumOfBooks() const noexcept -> size_t = 0;
    
    virtual auto filterBooks(BookFilters bookFilters) -> FilterResults = 0;
};

} // Repository namespace

#endif // IBOOK_REPOSITORY_HPP
