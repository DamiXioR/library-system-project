#ifndef BOOK_BUILDER_HPP
#define BOOK_BUILDER_HPP

#include "Book.hpp"
#include "BookId.hpp"
#include "UuidGenerator.hpp"
#include <vector>
#include <string>

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
    
    Book createWitcherTheLastWish() {
        return createAndrzejSapkowskiBook("The Witcher: The Last Wish", 1993);
    }

    Book createWitcherSwordOfDestiny() {
        return createAndrzejSapkowskiBook("The Witcher: Sword of Destiny", 1992);
    }

    Book createWitcherBloodOfElves() {
        return createAndrzejSapkowskiBook("The Witcher: Blood of Elves", 1994);
    }

    Book createWitcherTimeOfContempt() {
        return createAndrzejSapkowskiBook("The Witcher: Time of Contempt", 1995);
    }

    Book createWitcherBaptismOfFire() {
        return createAndrzejSapkowskiBook("The Witcher: Baptism of Fire", 1996);
    }

    Book createWitcherTowerOfSwallows() {
        return createAndrzejSapkowskiBook("The Witcher: The Tower of the Swallow", 1997);
    }

    Book createWitcherLadyOfTheLake() {
        return createAndrzejSapkowskiBook("The Witcher: The Lady of the Lake", 1999);
    }

    Book createWitcherSeasonOfStorms() {
        return createAndrzejSapkowskiBook("The Witcher: Season of Storms", 2013);
    }

    Book createQuoVadis() {
        return createHenrykSienkiewiczBook("Quo Vadis", 1896);
    }

    Book createWithFireAndSword() {
        return createHenrykSienkiewiczBook("With Fire and Sword", 1884);
    }

    Book createTheDeluge() {
        return createHenrykSienkiewiczBook("The Deluge", 1886);
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

    Book createAshes() {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Stefan Żeromski")
            .withTitle("Ashes")
            .withPublicationYear(1904)
            .build();
    }

    std::vector<Book> createBooks() {
        return {
            createDziady(),
            createHyperion(),
            create1984(),
            createHobbit(),
            createLordOfTheRings(),
            createSolaris(),
            createCyberiada(),
            createAshes()
        };
    }

private:
    Book createHenrykSienkiewiczBook(const std::string& title, int year) {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Henryk Sienkiewicz")
            .withTitle(std::move(title))
            .withPublicationYear(year)
            .build();
    }

    Book createAndrzejSapkowskiBook(const std::string& title, int year) {
        return withId(BookId{*UuidGenerator<BookId>::createUuid()})
            .withAuthor("Andrzej Sapkowski")
            .withTitle(std::move(title))
            .withPublicationYear(year)
            .build();
    }
};

}

#endif // BOOK_BUILDER_HPP