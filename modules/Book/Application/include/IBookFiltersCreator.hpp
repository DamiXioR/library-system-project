#ifndef I_DATA_CONTAINER_FILTER_CREATOR_HPP
#define I_DATA_CONTAINER_FILTER_CREATOR_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>

#include "Book.hpp"
#include "BookFilters.hpp"

namespace BookFiltering {

using FilterResults = std::unordered_set<std::weak_ptr<Book>, WeakBookHash, WeakBookEqual>;

class IBookFiltersCreator {
public:
    virtual ~IBookFiltersCreator() = default;

    virtual auto withTitleFilter(BookFilters& bookFilters, std::vector<std::string> titles) -> void = 0;
    virtual auto withAuthorFilter(BookFilters& bookFilters, std::vector<std::string> authors) -> void = 0;
    virtual auto withPublicationYearFilter(BookFilters& bookFilters, uint16_t minYear, uint16_t maxYear) -> void = 0;
};

} // namespace DataBase

#endif // I_DATA_CONTAINER_FILTER_CREATOR_HPP
