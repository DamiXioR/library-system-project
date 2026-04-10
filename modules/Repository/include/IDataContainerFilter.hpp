#ifndef I_DATA_CONTAINER_FILTER_HPP
#define I_DATA_CONTAINER_FILTER_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>

#include "Book.hpp"

namespace DataBase {

using FilteredBooks = std::unordered_set<std::weak_ptr<Book>, WeakBookHash, WeakBookEqual>;
using FilterQuery = std::unordered_map<std::string, std::vector<std::string>>;

class IDataContainerFilter {
public:
    virtual ~IDataContainerFilter() = default;
    virtual auto queryService(FilterQuery filterQuery) -> DataBase::FilteredBooks = 0;
};

} // namespace DataBase

#endif // I_DATA_CONTAINER_FILTER_HPP
