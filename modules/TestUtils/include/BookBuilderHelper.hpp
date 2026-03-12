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
    uint16_t m_publicationYear{966};

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

    BookBuilder& withPublicationYear(uint16_t pYear) {
        m_publicationYear = std::move(pYear);
        return *this;
    }

    Book build() const {
        Book book = Book(m_id, m_author, m_title);
        book.setPublicationYear(m_publicationYear);
        return book;
    }

    Book createDziady() {
    return withId(BookId{*UuidGenerator<BookId>::createUuid()})
        .withAuthor("Adam Mickiewicz")
        .withTitle("Dziady")
        .withPublicationYear(1823)
        .build();
    }

    Book createHyperion() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Dan Simmons")
            .withTitle("Hyperion")
            .withPublicationYear(1989)
            .build();
    }

    Book createWiedzminOstatnieZyczenie() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Andrzej Sapkowski")
            .withTitle("Wiedźmin. Ostatnie życzenie")
            .withPublicationYear(1993)
            .build();
    }

    Book createWiedzminMieczPrzeznaczenia() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Andrzej Sapkowski")
            .withTitle("Wiedźmin. Miecz Przeznaczenia.")
            .withPublicationYear(1992)
            .build();
    }

    Book create1984() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("George Orwell")
            .withTitle("1984")
            .withPublicationYear(1949)
            .build();
    }

    Book createHobbit() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("J. R. R. Tolkien")
            .withTitle("The Hobbit")
            .withPublicationYear(1937)
            .build();
    }

    Book createLordOfTheRings() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("J. R. R. Tolkien")
            .withTitle("The Lord of the Rings")
            .withPublicationYear(1954)
            .build();
    }

    Book createSolaris() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Stanisław Lem")
            .withTitle("Solaris")
            .withPublicationYear(1961)
            .build();
    }

    Book createCyberiada() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Stanisław Lem")
            .withTitle("Cyberiada")
            .withPublicationYear(1965)
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