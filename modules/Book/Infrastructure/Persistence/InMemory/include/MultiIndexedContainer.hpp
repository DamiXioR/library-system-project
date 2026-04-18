#ifndef MULTI_INDEXED_CONTAINER_HPP
#define MULTI_INDEXED_CONTAINER_HPP

#include <unordered_map>
#include <unordered_set>
#include <map>

#include <memory>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdint>
#include <cassert>
#include <optional>
#include <cwctype>

#include "Book.hpp"
#include "BookId.hpp"


namespace {
    // TODO: move to a separate file
    std::unordered_set<std::string> stopwords_en = {
        // articles
        "the",

        // conjunctions
        "and","or","but","if","while","although","because","since","unless",
        "until","whereas",

        // prepositions
        "of","at","by","for","with","about","against","between","into",
        "through","during","before","after","above","below","to","from",
        "up","down","in","out","on","off","over","under","again","further",

        // adverbs / misc
        "then","once","here","there","when","where","why","how",

        // quantifiers
        "all","any","both","each","few","more","most","other","some","such",

        // negation / modality
        "no","nor","not","only","own","same","so","than","too","very",
        "can","will","just","should","now","might","must","could","would",

        // verbs (auxiliary)
        "is","am","are","was","were","be","been","being",
        "have","has","had","having",
        "do","does","did","doing",

        // pronouns
        "this","that","these","those",
        "he","she","it","they","them","his","her","its","their","theirs",
        "you","your","yours","me","my","mine","we","our","ours","us",

        // question words
        "what","which","who","whom",

        // common filler words
        "also","even","still","yet","much","many","every","either","neither",

        // book-specific low-value words 🔥
        "story","stories","book","books","novel","novels",
        "guide","introduction","intro","introductionto",
        "collection","selected","edition","volume","vol",
        "part","series","set",

        // very generic academic fluff 🔥
        "approach","study","analysis","overview","concept",
        "theory","principle","principles","method","methods",
        "practice","practices","handbook","manual",

        // weak descriptive words 🔥
        "new","old","great","big","small","first","last",
        "early","late","modern","ancient","general","basic",
        "advanced","complete","essential","ultimate",

        // time/context junk 🔥
        "day","days","year","years","life","lives","world",

        // extremely common verbs (low signal)
        "make","makes","made",
        "take","takes","taken",
        "get","gets","got",
        "go","goes","went",
        "see","sees","seen",
        "know","knows","known",
        "find","finds","found"
    };
} // anonymous namespace

namespace DataBase {

using PrimaryContainer = 
    std::unordered_map<BookId, std::shared_ptr<Book>, BookId::BookIdHash>;

using SecondaryTextIndex =
    std::unordered_map<std::string,
        std::unordered_set<std::weak_ptr<Book>, WeakBookHash, WeakBookEqual>>;

using SecondaryUint16tIndexOrdered =
    std::map<std::optional<uint16_t>,
        std::unordered_set<std::weak_ptr<Book>, WeakBookHash, WeakBookEqual>>;

class MultiIndexedContainer {
public:
    MultiIndexedContainer() = default;

    MultiIndexedContainer(
        std::unique_ptr<PrimaryContainer> primary,
        std::unique_ptr<SecondaryTextIndex> title,
        std::unique_ptr<SecondaryTextIndex> titleTokens,
        std::unique_ptr<SecondaryTextIndex> author,
        std::unique_ptr<SecondaryUint16tIndexOrdered> year)
        : m_primaryContainer(std::move(primary)),
          m_titleIndex(std::move(title)),
          m_tokensFromTitle(std::move(title)),
          m_authorIndex(std::move(author)),
          m_yearIndex(std::move(year)) {}

    const PrimaryContainer& getPrimaryContainer() const { return *m_primaryContainer; }
    const SecondaryTextIndex& getTitleIndex() const { return *m_titleIndex; }
    const SecondaryTextIndex& getTokensFromTitle() const { return *m_tokensFromTitle; }
    const SecondaryTextIndex& getAuthorIndex() const { return *m_authorIndex; }
    const SecondaryUint16tIndexOrdered& getYearIndex() const { return *m_yearIndex; }

    auto addBook(Book book) -> bool {
        auto bookPtr = std::make_shared<Book>(book);
        auto bookId = bookPtr->getBookId();

        auto [it, inserted] = m_primaryContainer->emplace(bookId, bookPtr);
        if (!inserted) return false; // element actually exists

        auto [itTitle, _] = m_titleIndex->try_emplace(bookPtr->getTitle());
        itTitle->second.insert(bookPtr);

        auto [itBook, _] = m_authorIndex->try_emplace(bookPtr->getAuthor());
        itBook->second.insert(bookPtr);

        auto [itYear, _] = m_yearIndex->try_emplace(bookPtr->getPublicationYear());
        itYear->second.insert(bookPtr);

        fillSplitTitleWordsContainer(bookPtr);

        return true;
    }

    auto removeBook(const BookId& bookId) -> bool {
        auto primContainerIt = m_primaryContainer->find(bookId);
        if (primContainerIt == m_primaryContainer->end()) {
            return false;
        }

        std::shared_ptr<Book> bookToRemove = primContainerIt->second;

        auto result {true};
        result = removeFromSecondaryContainerMap(m_titleIndex.get(), bookToRemove->getTitle(), bookToRemove);
        if(!result) return result;
        result = removeFromSecondaryContainerMap(m_authorIndex.get(), bookToRemove->getAuthor(), bookToRemove);
        if(!result) return result;
        result = removeFromSecondaryContainerMap(m_yearIndex.get(), bookToRemove->getPublicationYear(), bookToRemove);
        if(!result) return result;

        const auto splitedTitle {splitTitleIntoTokens(bookToRemove)};
        for(const auto& word : splitedTitle){
            result = removeFromSecondaryContainerMap(m_tokensFromTitle.get(), word, bookToRemove);
            if(!result) return result;
        }

        uint16_t noBookRemoved {0};
        if(auto eraseCount = m_primaryContainer->erase(primContainerIt->first); eraseCount == noBookRemoved){
            return false;
        };

        return result;
    }

    auto getContainerSize() const noexcept -> size_t {
        return m_primaryContainer->size();
    }

private:
    std::unique_ptr<PrimaryContainer> m_primaryContainer = std::make_unique<PrimaryContainer>();
    std::unique_ptr<SecondaryTextIndex> m_titleIndex = std::make_unique<SecondaryTextIndex>();
    std::unique_ptr<SecondaryTextIndex> m_tokensFromTitle = std::make_unique<SecondaryTextIndex>();
    std::unique_ptr<SecondaryTextIndex> m_authorIndex = std::make_unique<SecondaryTextIndex>();
    std::unique_ptr<SecondaryUint16tIndexOrdered> m_yearIndex = std::make_unique<SecondaryUint16tIndexOrdered>();

    template <class SecondaryContainerType, class MainKeyType, class ValueToRemoveType>
    auto removeFromSecondaryContainerMap(SecondaryContainerType* secContainer, const MainKeyType mainKey, const ValueToRemoveType valueToRemove) -> bool {
        
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

    auto splitTitleIntoTokens(std::shared_ptr<Book> bookPtr) -> std::vector<std::string> {
        auto title = bookPtr->getTitle();
        std::transform(title.begin(), title.end(), title.begin(), tolower);

        std::vector<std::string> titleTokens {};
        std::size_t vectorCapacityFactor {2};
        titleTokens.reserve(title.size() / vectorCapacityFactor);

        std::string token {};
        bool previousSignWasEmpty {false};
        std::size_t smallWordsFactor {1};

        for(const auto& sign : title){
            if(iswalnum(sign)){
                previousSignWasEmpty = false;
                token += sign;
            }
            if(sign == ' ' && !previousSignWasEmpty){
                previousSignWasEmpty = true;
                if(token.size() > smallWordsFactor && !stopwords_en.contains(token)){
                    titleTokens.push_back(token);
                }
                token.clear();
                continue;
            }
        }
        if(!token.empty() && token.size() > smallWordsFactor && !stopwords_en.contains(token)){
            titleTokens.push_back(token);
        }
        
        return titleTokens;
    }

    auto insertSplitedTitleIntoTitleWordsContainer(std::vector<std::string> splitedTitle, std::shared_ptr<Book> bookPtr) -> void {
        for(const auto word : splitedTitle){
         auto [itTitle, _] = m_tokensFromTitle->try_emplace(word);
         itTitle->second.insert(bookPtr);
        }
    }
    
    auto fillSplitTitleWordsContainer(std::shared_ptr<Book> bookPtr) -> void {
        const auto splitedTitle {splitTitleIntoTokens(bookPtr)};
        insertSplitedTitleIntoTitleWordsContainer(splitedTitle, bookPtr);
    }
 
};

} // namespace DataBase

#endif // MULTI_INDEXED_CONTAINER_HPP
