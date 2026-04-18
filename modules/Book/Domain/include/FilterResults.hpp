#ifndef FILTER_RESULTS_HPP
#define FILTER_RESULTS_HPP

#include <unordered_set>
#include <memory>

#include "Book.hpp"

class FilterResults {
public:
    std::unordered_set<std::weak_ptr<Book>, WeakBookHash, WeakBookEqual> m_books;
};

#endif // FILTER_RESULTS_HPP