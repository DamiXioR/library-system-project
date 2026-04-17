#ifndef FILTER_MULTI_INDEXED_CONTAINER_HPP
#define FILTER_MULTI_INDEXED_CONTAINER_HPP

#include <unordered_set>

#include <string>
#include <cstdint>
#include <ranges>

#include "Book.hpp"
#include "BookId.hpp"
#include "IBookFilteringForMIC.hpp"
#include "MultiIndexedContainer.hpp"
#include "BookFilters.hpp"

namespace DataBase {

constexpr uint16_t BASE_FILTERED_BOOKID_SIZE {20};

class BookFilteringForMIC : public IBookFilteringForMIC {
public:
    BookFilteringForMIC(MultiIndexedContainer* mic) : m_micForFilter(mic) {}

    auto filterService(BookFilters bookFilters) -> DataBase::FilteredBooks {
        return startFiltering(std::move(bookFilters));
    }

    auto swapMic(MultiIndexedContainer* micToFilter) -> BookFilteringForMIC& {
        if(!m_micForFilter || (m_micForFilter && m_micForFilter != micToFilter)){
            m_micForFilter = micToFilter;
        }
        return *this;
    }

    template <class SecContainerType>
    auto fitlerTextIndexedContainer(SecContainerType secContainer, std::vector<std::string> data) -> FilteredBooks {
        FilteredBooks filteredBooks;
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
        return filteredBooks;
    }

    template <class SecContainerType, class NumType>
    auto filterNumIndexedContainer(SecContainerType secContainer, NumType min, NumType max) -> FilteredBooks {
        FilteredBooks filteredBooks;
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
        return filteredBooks;
    }

    auto startFiltering(BookFilters bookFilters) -> FilteredBooks {
        FilteredBooks filteredBooks;
        filteredBooks.reserve(BASE_FILTERED_BOOKID_SIZE);
        auto isPrimaryContainerUsed {false};

        if(auto& [isTitleFilterApplied, titles] = bookFilters.titleFilter; isTitleFilterApplied){
            filteredBooks = fitlerTextIndexedContainer(m_micForFilter->getTitleIndex(), titles);
            isTitleFilterApplied = false;
            isPrimaryContainerUsed = true;
        }
        if(auto& [isAuthorFilterApplied, authors] = bookFilters.authorFilter; isAuthorFilterApplied && !isPrimaryContainerUsed){
            filteredBooks = fitlerTextIndexedContainer(m_micForFilter->getAuthorIndex(), authors);
            isAuthorFilterApplied = false;
            isPrimaryContainerUsed = true;
            
        }
        if(auto& [isPubYearFilterApplied, yearRange] = bookFilters.pYearFilter; isPubYearFilterApplied && !isPrimaryContainerUsed){
            filteredBooks = filterNumIndexedContainer(m_micForFilter->getYearIndex(), yearRange.first, yearRange.second);
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

        return filteredBooks
                | std::views::filter(makeEqualityFilter(
                    bookFilters.titleFilter,
                    [](const auto& b) -> const auto& { return b->getTitle(); }
                ))
                | std::views::filter(makeEqualityFilter(
                    bookFilters.authorFilter,
                    [](const auto& b) -> const auto& { return b->getAuthor(); }
                ))
                | std::views::filter(usePublYearFilter)
                | std::ranges::to<FilteredBooks>();
    }

private:
    MultiIndexedContainer* m_micForFilter {nullptr};
};

} // namespace DataBase

#endif // FILTER_MULTI_INDEXED_CONTAINER_HPP
