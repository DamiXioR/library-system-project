#ifndef BOOK_FILTERS_HPP
#define BOOK_FILTERS_HPP

#include <vector>
#include <string>
#include <functional>
#include <cstdint>

class BookFilters {
public:
    std::pair<bool, std::vector<std::string>> titleFilter;
    std::pair<bool, std::vector<std::string>> authorFilter;
    std::pair<bool, std::pair<uint16_t, uint16_t>> pYearFilter;
};

#endif // BOOK_FILTERS_HPP
