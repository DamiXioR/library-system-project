#ifndef FILTER_MULTI_INDEXED_CONTAINER_CREATOR_HPP
#define FILTER_MULTI_INDEXED_CONTAINER_CREATOR_HPP

#include <string>
#include <cstdint>
#include <functional>

#include "IBookFiltersCreator.hpp"
#include "BookFilters.hpp"

namespace BookFiltering {

class BookFiltersCreator : IBookFiltersCreator {
public:
    BookFiltersCreator() = default;

    auto withTitleFilter(BookFilters& bookFilters, const std::vector<std::string>& titles) -> void {
        bookFilters.titleFilter = std::move(titles);
    }

    auto withAuthorFilter(BookFilters& bookFilters, const std::vector<std::string>& authors) -> void {
        bookFilters.authorFilter = std::move(authors);
    }

    auto withPublicationYearFilter(BookFilters& bookFilters, uint16_t minYear, uint16_t maxYear) -> void {
        bookFilters.pYearFilter = std::make_optional(PublYear{minYear, maxYear});
    }
};

} // namespace BookFiltering

#endif // FILTER_MULTI_INDEXED_CONTAINER_CREATOR_HPP
