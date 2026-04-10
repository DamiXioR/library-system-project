#ifndef I_DATA_CONTAINER_HPP
#define I_DATA_CONTAINER_HPP


#include "Book.hpp"
#include "BookId.hpp"

namespace DataBase {

class IDataContainer {
public:
    virtual ~IDataContainer() = default;

    virtual auto addBook(Book book) -> bool = 0;
    virtual auto removeBook(const BookId& bookId) -> bool = 0;
    virtual auto getContainerSize() const noexcept -> size_t = 0;
};

} // namespace DataBase

#endif // I_DATA_CONTAINER_HPP
