#ifndef BOOK_FILTERS_HPP
#define BOOK_FILTERS_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <optional>

struct PublYear {
    uint16_t minYear;
    uint16_t maxYear;
};

struct BookFilters {
    std::vector<std::string> titleFilter;
    std::vector<std::string> authorFilter;
    std::optional<PublYear> pYearFilter;
};

#endif // BOOK_FILTERS_HPP
