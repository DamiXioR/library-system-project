#include <string>
#include <algorithm>
#include <memory>
#include <cstdint>
#include <vector>

#include "Book.hpp"
#include "BookId.hpp"
#include "BookRepository.hpp"

auto BookRepository::getBookById(const BookId& id) const noexcept -> std::optional<std::reference_wrapper<const Book>> {
    const auto found {m_books.find(id)};
    if(found != m_books.end()){
        return found->second;
    }
    return std::nullopt;
}

auto BookRepository::getBookByTitle(const std::string& title) const noexcept -> std::vector<std::reference_wrapper<const Book>> {
    auto books = std::vector<std::reference_wrapper<const Book>>();
    uint16_t reservationCount {10};
    books.reserve(reservationCount);

    for(const auto& [_, book] : m_books){
        if(book.getTitle() == title){
            books.push_back(book);
        }
    }

    return books;
}

auto BookRepository::getBookByAuthor(const std::string& author) const noexcept -> std::vector<std::reference_wrapper<const Book>> {
    auto books = std::vector<std::reference_wrapper<const Book>>();
    uint16_t reservationCount {10};
    books.reserve(reservationCount);

    for(const auto& [_, book] : m_books){
        if(book.getAuthor() == author){
            books.push_back(book);
        }
    }

    return books;
}

auto BookRepository::addBook(const Book& book) noexcept -> bool {
    auto [it, isInserted] = m_books.insert({book.getBookId(), book});
    return isInserted;
}

auto BookRepository::removeBook(const BookId& bookId) noexcept -> bool {
    return m_books.erase(bookId);
}

auto BookRepository::getNumOfBooks() const noexcept -> size_t {
    return m_books.size();
}
