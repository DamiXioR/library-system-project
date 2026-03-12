#ifndef MULTI_INDEXED_CONTAINER_HPP
#define MULTI_INDEXED_CONTAINER_HPP

#include <unordered_map>
#include <unordered_set>

#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include <cassert>
#include <optional>

#include "Book.hpp"
#include "BookId.hpp"

namespace DataBase {

struct WeakBookHash {
    auto operator()(const std::weak_ptr<Book>& wp) const noexcept -> size_t {
        auto sp = wp.lock();
        return sp ? std::hash<Book*>()(sp.get()) : 0;
    }
};

struct WeakBookEqual {
    auto operator()(const std::weak_ptr<Book>& first, const std::weak_ptr<Book>& second) const -> bool {
        auto sFirst = first.lock();
        auto sSecond = second.lock();
        if (!sFirst || !sSecond) return false;
        return sFirst->getBookId().getBookId() == sSecond->getBookId().getBookId();
    }
};

using PrimaryContainer = 
    std::unordered_map<BookId, std::shared_ptr<Book>, BookId::BookIdHash>;

using SecondaryTextIndex =
    std::unordered_map<std::string,
        std::unordered_set<std::weak_ptr<Book>, WeakBookHash, WeakBookEqual>>;

using SecondaryNumIndex =
    std::unordered_map<std::optional<uint16_t>,
        std::unordered_set<std::weak_ptr<Book>, WeakBookHash, WeakBookEqual>>;

class MultiIndexedContainer {
public:
    MultiIndexedContainer() = default;

    MultiIndexedContainer(
        std::unique_ptr<PrimaryContainer> primary,
        std::unique_ptr<SecondaryTextIndex> title,
        std::unique_ptr<SecondaryTextIndex> author,
        std::unique_ptr<SecondaryNumIndex> year)
        : m_primaryContainer(std::move(primary)),
          m_titleIndex(std::move(title)),
          m_authorIndex(std::move(author)),
          m_yearIndex(std::move(year)) {}

    const PrimaryContainer& getPrimaryContainer() const { return *m_primaryContainer; }
    const SecondaryTextIndex& getTitleIndex() const { return *m_titleIndex; }
    const SecondaryTextIndex& getAuthorIndex() const { return *m_authorIndex; }
    const SecondaryNumIndex& getYearIndex() const { return *m_yearIndex; }

    auto addBook(const BookId& bookId, const Book& book) -> bool {
        auto bookPtr = std::make_shared<Book>(book);

        auto [it, inserted] = m_primaryContainer->emplace(bookId, bookPtr);
        if (!inserted) return false; // element actually exists

        auto [itTitle, _] = m_titleIndex->try_emplace(book.getTitle());
        itTitle->second.insert(bookPtr);

        auto [itBook, _] = m_authorIndex->try_emplace(book.getAuthor());
        itBook->second.insert(bookPtr);

        auto [itYear, _] = m_yearIndex->try_emplace(book.getPublicationYear());
        itYear->second.insert(bookPtr);

        return true;
    }

    bool removeBook(const BookId& bookId) {
        auto result = true;

        auto primContainerIt = m_primaryContainer->find(bookId);
        if (primContainerIt == m_primaryContainer->end()) {
            return !result;
        }

        std::shared_ptr<Book> bookToRemove = primContainerIt->second;

        result = removeFromSecondaryContainerMap(m_titleIndex.get(), bookToRemove->getTitle(), bookToRemove);
        result = removeFromSecondaryContainerMap(m_authorIndex.get(), bookToRemove->getAuthor(), bookToRemove);
        result = removeFromSecondaryContainerMap(m_yearIndex.get(), bookToRemove->getPublicationYear(), bookToRemove);

        m_primaryContainer->erase(primContainerIt);

        return result;
    }

private:
    std::unique_ptr<PrimaryContainer> m_primaryContainer = std::make_unique<PrimaryContainer>();
    std::unique_ptr<SecondaryTextIndex> m_titleIndex = std::make_unique<SecondaryTextIndex>();
    std::unique_ptr<SecondaryTextIndex> m_authorIndex = std::make_unique<SecondaryTextIndex>();
    std::unique_ptr<SecondaryNumIndex> m_yearIndex = std::make_unique<SecondaryNumIndex>();

    template <class SecondaryContainerType, class MainKeyType, class ValueToRemoveType>
    bool removeFromSecondaryContainerMap(SecondaryContainerType* secContainer, const MainKeyType mainKey, const ValueToRemoveType valueToRemove) {
        
        auto rowIt = secContainer->find(mainKey);
        if (rowIt == secContainer->end()) return false;

        auto foundBookIt = rowIt->second.find(valueToRemove);
        if (foundBookIt == rowIt->second.end()) return false;

        auto& insideUnorderedMap = rowIt->second;
        insideUnorderedMap.erase(foundBookIt);
        if (insideUnorderedMap.empty()){
            secContainer->erase(rowIt);
        }

        return true;
    }

    template<typename Index, typename Key>
    static std::vector<std::shared_ptr<Book>> collectValid(const Index& index, const Key& key) {
        std::vector<std::shared_ptr<Book>> result;
        auto it = index.find(key);
        if (it == index.end()) return result;

        for (auto& weakBook : it->second) {
            if (auto sp = weakBook.lock()) result.push_back(sp);
        }
        return result;
    }
};

} // namespace DataBase

#endif // MULTI_INDEXED_CONTAINER_HPP
