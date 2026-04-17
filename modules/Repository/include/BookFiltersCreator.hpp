#ifndef FILTER_MULTI_INDEXED_CONTAINER_CREATOR_HPP
#define FILTER_MULTI_INDEXED_CONTAINER_CREATOR_HPP

#include <string>
#include <cstdint>
#include <functional>

#include "IBookFiltersCreator.hpp"
#include "BookFilters.hpp"

namespace DataBase {

class BookFiltersCreator : IBookFiltersCreator {
public:
    BookFiltersCreator() = default;

    auto withTitleFilter(BookFilters& bookFilters, std::vector<std::string> titles) -> void {
        bookFilters.titleFilter = std::make_pair(true, titles);
    }

    auto withAuthorFilter(BookFilters& bookFilters, std::vector<std::string> authors) -> void {
        bookFilters.authorFilter = std::make_pair(true, authors);
    }

    auto withPublicationYearFilter(BookFilters& bookFilters, uint16_t minYear, uint16_t maxYear) -> void {
        bookFilters.pYearFilter = std::make_pair(true, std::make_pair(minYear, maxYear));
    }
};

} // namespace DataBase

#endif // FILTER_MULTI_INDEXED_CONTAINER_CREATOR_HPP
