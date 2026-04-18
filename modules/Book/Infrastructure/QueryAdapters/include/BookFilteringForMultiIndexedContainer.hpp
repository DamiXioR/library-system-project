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

        if(auto& [isTitleFilterApplied, titles] = bookFilters.titleFilter; isTitleFilterApplied){
            filterResults = fitlerTextIndexedContainer(m_micForFilter->getTitleIndex(), titles);
            isTitleFilterApplied = false;
            isPrimaryContainerUsed = true;
        }
        if(auto& [isAuthorFilterApplied, authors] = bookFilters.authorFilter; isAuthorFilterApplied && !isPrimaryContainerUsed){
            filterResults = fitlerTextIndexedContainer(m_micForFilter->getAuthorIndex(), authors);
            isAuthorFilterApplied = false;
            isPrimaryContainerUsed = true;
            
        }
        if(auto& [isPubYearFilterApplied, yearRange] = bookFilters.pYearFilter; isPubYearFilterApplied && !isPrimaryContainerUsed){
            filterResults = filterNumIndexedContainer(m_micForFilter->getYearIndex(), yearRange.first, yearRange.second);
            isPubYearFilterApplied = false;
            isPrimaryContainerUsed = true;
        }

        auto makeEqualityFilter = [](auto filter, auto getter) -> auto {
            return [filter, getter](const auto& weakBook) -> bool {
                if (!filter.first) return true;

                auto book = weakBook.lock();
                if (!book) return false;

                return std::ranges::any_of(filter.second, [&](const auto& v) {
                    return getter(book) == v;
                });
            };
        };

        auto usePublYearFilter = [&bookFilters](auto& weekBook) -> bool {
            if(auto [isPubYearFilterApplied, yearRange] = bookFilters.pYearFilter; isPubYearFilterApplied){
                auto book {weekBook.lock()};
                if (!book) return false;
                if(book->getPublicationYear() >= yearRange.first && book->getPublicationYear() <= yearRange.second){
                        return true;
                }
                return false; 
            }
            return true;
        };

        auto result =
            filterResults.m_books
            | std::views::filter(makeEqualityFilter(
                bookFilters.titleFilter,
                [](const auto& b) -> const auto& { return b->getTitle(); }
            ))
            | std::views::filter(makeEqualityFilter(
                bookFilters.authorFilter,
                [](const auto& b) -> const auto& { return b->getAuthor(); }
            ))
            | std::views::filter(usePublYearFilter)
            | std::ranges::to<decltype(filterResults.m_books)>();

        filterResults.m_books = std::move(result);

        return filterResults;
    }

private:
    DataBase::MultiIndexedContainer* m_micForFilter {nullptr};
};

} // namespace BookFiltering

#endif // FILTER_MULTI_INDEXED_CONTAINER_HPP
