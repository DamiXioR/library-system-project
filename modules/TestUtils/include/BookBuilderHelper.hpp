#ifndef BOOK_BUILDER_HPP
#define BOOK_BUILDER_HPP

#include "Book.hpp"
#include "BookId.hpp"
#include "UuidGenerator.hpp"
#include <vector>

namespace BookHelpers {

class BookBuilder {
private:
    BookId m_id{"123-456-789-101112"};
    std::string m_title{"Default Title"};
    std::string m_author{"Default Author"};

public:
    BookBuilder& withId(BookId id) {
        m_id = std::move(id);
        return *this;
    }

    BookBuilder& withTitle(std::string title) {
        m_title = std::move(title);
        return *this;
    }

    BookBuilder& withAuthor(std::string author) {
        m_author = std::move(author);
        return *this;
    }

    Book build() const {
        return Book(m_id, m_author, m_title);
    }

    Book createDziady() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Adam Mickiewicz")
            .withTitle("Dziady")
            .build();
    }

    Book createHyperion() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Dan Simmons")
            .withTitle("Hyperion")
            .build();
    }

    Book createWiedzminOstatnieZyczenie() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Andrzej Sapkowski")
            .withTitle("Wiedźmin. Ostatnie życzenie")
            .build();
    }

    Book createWiedzminMieczPrzeznaczenia() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Andrzej Sapkowski")
            .withTitle("Wiedźmin. Miecz Przeznaczenia.")
            .build();
    }

    Book create1984() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("George Orwell")
            .withTitle("1984")
            .build();
    }

    Book createHobbit() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("J. R. R. Tolkien")
            .withTitle("The Hobbit")
            .build();
    }

    Book createLordOfTheRings() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("J. R. R. Tolkien")
            .withTitle("The Lord of the Rings")
            .build();
    }

    Book createSolaris() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Stanisław Lem")
            .withTitle("Solaris")
            .build();
    }

    Book createCyberiada() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Stanisław Lem")
            .withTitle("Cyberiada")
            .build();
    }

    std::vector<Book> createBooks() {
        return {
            createDziady(),
            createHyperion(),
            createWiedzminOstatnieZyczenie(),
            create1984(),
            createHobbit(),
            createLordOfTheRings(),
            createSolaris(),
            createCyberiada()
        };
    }
};

}

#endif // BOOK_BUILDER_HPP