#ifndef FILTER_MULTI_INDEXED_CONTAINER_HPP
#define FILTER_MULTI_INDEXED_CONTAINER_HPP

#include <unordered_set>

#include <string>
#include <cstdint>
#include <ranges>

#include "Book.hpp"
#include "BookId.hpp"
#include "MultiIndexedContainer.hpp"
#include "BookFilters.hpp"
#include "FilterResults.hpp"

namespace BookFiltering {

constexpr uint16_t BASE_FILTERED_BOOKID_SIZE {20};

class BookFilteringForMultiIndexedContainer {
public:
    BookFilteringForMultiIndexedContainer(DataBase::MultiIndexedContainer* mic) : m_micForFilter(mic) {}

    auto filterService(BookFilters bookFilters) -> FilterResults {
        return startFiltering(std::move(bookFilters));
    }

    auto swapMic(DataBase::MultiIndexedContainer* micToFilter) -> BookFilteringForMultiIndexedContainer& {
        if(!m_micForFilter || (m_micForFilter && m_micForFilter != micToFilter)){
            m_micForFilter = micToFilter;
        }
        return *this;
    }

    template <class SecContainerType>
    auto fitlerTextIndexedContainer(SecContainerType secContainer, std::vector<std::string> data) -> FilterResults {
        FilterResults filterResults;
        auto& filteredBooks {filterResults.m_books};
        filteredBooks.reserve(BASE_FILTERED_BOOKID_SIZE);
        for(const auto& item : data){
            const auto foundIt {secContainer.find(item)};
            if(foundIt != secContainer.end()){
                for (const auto& weakBook : foundIt->second) {
                    if (auto book = weakBook.lock()) {
                        filteredBooks.insert(book);
                    }
                }
            }
        }
        return filterResults;
    }

    template <class SecContainerType, class NumType>
    auto filterNumIndexedContainer(SecContainerType secContainer, NumType min, NumType max) -> FilterResults {
        FilterResults filterResults;
        auto& filteredBooks {filterResults.m_books};
        filteredBooks.reserve(BASE_FILTERED_BOOKID_SIZE);
        auto itLow = secContainer.lower_bound(min);
        auto itHigh = secContainer.upper_bound(max);
        for (auto it = itLow; it != itHigh; ++it) {
            auto booksUnderDate {it->second};
            for(const auto& weakBook : booksUnderDate){
                if (auto book = weakBook.lock()) {
                        filteredBooks.insert(book);
                    }
            }
        }
        return filterResults;
    }

    auto startFiltering(BookFilters bookFilters) -> FilterResults {
        FilterResults filterResults;
        auto isPrimaryContainerUsed {false};

        if(!bookFilters.titleFilter.empty()){
            filterResults = fitlerTextIndexedContainer(m_micForFilter->getTitleIndex(), bookFilters.titleFilter);
            isPrimaryContainerUsed = true;
        }
        if(!bookFilters.authorFilter.empty() && !isPrimaryContainerUsed){
            filterResults = fitlerTextIndexedContainer(m_micForFilter->getAuthorIndex(), bookFilters.authorFilter);
            isPrimaryContainerUsed = true;
            
        }
        if(bookFilters.pYearFilter.has_value() && !isPrimaryContainerUsed){
            const auto& publYear {bookFilters.pYearFilter.value()};
            filterResults = filterNumIndexedContainer(m_micForFilter->getYearIndex(), publYear.minYear, publYear.maxYear);
            isPrimaryContainerUsed = true;
        }

       auto matchesTitle = [&](const auto& weakBook) {
            if (bookFilters.titleFilter.empty())
                return true;

            auto book = weakBook.lock();
            if (!book)
                return false;

            return std::ranges::contains(
                bookFilters.titleFilter,
                book->getTitle()
            );
        };

        auto matchesAuthor = [&](const auto& weakBook) {
            if (bookFilters.authorFilter.empty())
                return true;

            auto book = weakBook.lock();
            if (!book)
                return false;

            return std::ranges::contains(
                bookFilters.authorFilter,
                book->getAuthor()
            );
        };

        auto matchesPublicationYear = [&](const auto& weakBook) {
            if (!bookFilters.pYearFilter.has_value())
                return true;

            auto book = weakBook.lock();
            if (!book)
                return false;

            const auto& yearFilter = bookFilters.pYearFilter.value();
            const auto year = book->getPublicationYear();

            return year >= yearFilter.minYear
                && year <= yearFilter.maxYear;
        };

        filterResults.m_books =
            filterResults.m_books
            | std::views::filter(matchesTitle)
            | std::views::filter(matchesAuthor)
            | std::views::filter(matchesPublicationYear)
            | std::ranges::to<decltype(filterResults.m_books)>();

        return filterResults;
    }

private:
    DataBase::MultiIndexedContainer* m_micForFilter {nullptr};
};

} // namespace BookFiltering

#endif // FILTER_MULTI_INDEXED_CONTAINER_HPP
