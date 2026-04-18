#ifndef MOCK_IBOOK_REPOSITORY_HPP
#define MOCK_IBOOK_REPOSITORY_HPP

#include <gmock/gmock.h>
#include "IBookRepository.hpp"

namespace Repository {

class MockIBookRepository : public IBookRepository {
public:
    MOCK_METHOD(bool, addBook, (Book book), (noexcept, override));
    MOCK_METHOD(bool, removeBook, (const BookId& bookId), (noexcept, override));
    MOCK_METHOD(size_t, getNumOfBooks, (), (const, noexcept, override));
    MOCK_METHOD(FilterResults, filterBooks, (BookFilters bookFilters), (override));
};

} // namespace Repository

#endif // MOCK_IBOOK_REPOSITORY_HPP