#ifndef LIBRARY_SERVICE_HPP
#define LIBRARY_SERVICE_HPP

#include <memory>
#include <vector>
#include <optional>
#include <functional>

#include "IBookRepository.hpp"
#include "AddBookCommand.hpp"
#include "RemoveBookCommand.hpp"
#include "BookFilters.hpp"
#include "FilterResults.hpp"

class LibraryService {
public:
    explicit LibraryService(std::unique_ptr<Repository::IBookRepository> bookRepo)
        : m_BookRepo(std::move(bookRepo)) {}

    auto addBook(const Actions::AddBookCommand& addBookCommand) -> bool {
        Book book {
            BookId(std::string{"123"}),
            addBookCommand.m_title,
            addBookCommand.m_author
        };
        book.setPublicationYear(addBookCommand.m_publicationYear);

        return m_BookRepo->addBook(book);
    }

    auto removeBook(const Actions::RemoveBookCommand& removeBookCommand) -> bool {
        const BookId& bookId {removeBookCommand.m_bookId};
        return m_BookRepo->removeBook(bookId);
    };

    auto getNumOfBooks() const noexcept -> size_t {
        return m_BookRepo->getNumOfBooks();
    }

    auto filterBooks(BookFilters bookFilters) -> FilterResults {
        return m_BookRepo->filterBooks(std::move(bookFilters));
    }


private:
    std::unique_ptr<Repository::IBookRepository> m_BookRepo;
};

#endif // LIBRARY_SERVICE_HPP
