#ifndef FILTER_MULTI_INDEXED_CONTAINER_HPP
#define FILTER_MULTI_INDEXED_CONTAINER_HPP

#include <unordered_set>

#include <string>
#include <cstdint>
#include <ranges>

#include "Book.hpp"
#include "BookId.hpp"
#include "IDataContainerFilter.hpp"
#include "MultiIndexedContainer.hpp"

namespace DataBase {

constexpr uint16_t BASE_FILTERED_BOOKID_SIZE {20};

class FilterMultiIndexedContainer : public IDataContainerFilter {
public:
    FilterMultiIndexedContainer() {
        appliedFilters.resetFilters();
        mic = nullptr;
    }

    auto queryService(FilterQuery filterQuery) -> DataBase::FilteredBooks {
        return {};
    }

    auto filterStart(MultiIndexedContainer* micToFilter) -> FilterMultiIndexedContainer& {
        if(!mic || (mic && mic != micToFilter)){
            mic = micToFilter;
        }

        appliedFilters.resetFilters();

        return *this;
    }

    auto withTitleFilter(std::initializer_list<std::string> titles) -> FilterMultiIndexedContainer& {
        appliedFilters.titleFilter = std::make_pair(true, titles);
        return *this;
    }

    auto withAuthorFilter(std::vector<std::string> authors) -> FilterMultiIndexedContainer& {
        appliedFilters.authorFilter = std::make_pair(true, authors);
        return *this;
    }

    auto withPublicationYearFilter(uint16_t minYear, uint16_t maxYear) -> FilterMultiIndexedContainer& {
        appliedFilters.pYearFilter = std::make_pair(true, std::make_pair(minYear, maxYear));
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

    auto applyFilters() -> FilteredBooks {
        FilteredBooks filteredBooks;
        filteredBooks.reserve(BASE_FILTERED_BOOKID_SIZE);
        auto isPrimaryContainerUsed {false};

        if(auto& [isTitleFilterApplied, titles] = appliedFilters.titleFilter; isTitleFilterApplied){
            filteredBooks = fitlerTextIndexedContainer(mic->getTitleIndex(), titles);
            isTitleFilterApplied = false;
            isPrimaryContainerUsed = true;
        }
        if(auto& [isAuthorFilterApplied, authors] = appliedFilters.authorFilter; isAuthorFilterApplied && !isPrimaryContainerUsed){
            filteredBooks = fitlerTextIndexedContainer(mic->getAuthorIndex(), authors);
            isAuthorFilterApplied = false;
            isPrimaryContainerUsed = true;
            
        }
        if(auto& [isPubYearFilterApplied, yearRange] = appliedFilters.pYearFilter; isPubYearFilterApplied && !isPrimaryContainerUsed){
            filteredBooks = filterNumIndexedContainer(mic->getYearIndex(), yearRange.first, yearRange.second);
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

        auto usePublYearFilter = [*this](auto& weekBook) -> bool {
            if(auto [isPubYearFilterApplied, yearRange] = appliedFilters.pYearFilter; isPubYearFilterApplied){
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
                    appliedFilters.titleFilter,
                    [](const auto& b) -> const auto& { return b->getTitle(); }
                ))
                | std::views::filter(makeEqualityFilter(
                    appliedFilters.authorFilter,
                    [](const auto& b) -> const auto& { return b->getAuthor(); }
                ))
                | std::views::filter(usePublYearFilter)
                | std::ranges::to<FilteredBooks>();
    }

private:
    class AppliedFilters {
    public:
        std::pair<bool, std::vector<std::string>> titleFilter;
        std::pair<bool, std::vector<std::string>> authorFilter;
        std::pair<bool, std::pair<uint16_t, uint16_t>> pYearFilter;

        auto resetFilters() -> void{
            titleFilter = {false, {}};
            authorFilter = {false, {}};
            pYearFilter = {false, {}};
        }

    } appliedFilters;

    MultiIndexedContainer* mic {nullptr};
};

} // namespace DataBase

#endif // FILTER_MULTI_INDEXED_CONTAINER_HPP
