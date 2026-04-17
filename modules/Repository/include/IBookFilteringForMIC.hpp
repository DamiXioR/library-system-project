#ifndef I_DATA_CONTAINER_FILTER_HPP
#define I_DATA_CONTAINER_FILTER_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>

#include "Book.hpp"
#include "BookFilters.hpp"

namespace DataBase {

using FilteredBooks = std::unordered_set<std::weak_ptr<Book>, WeakBookHash, WeakBookEqual>;

class IBookFilteringForMIC {
public:
    virtual ~IBookFilteringForMIC() = default;
    virtual auto filterService(BookFilters bookFilters) -> DataBase::FilteredBooks = 0;
};

} // namespace DataBase

#endif // I_DATA_CONTAINER_FILTER_HPP
