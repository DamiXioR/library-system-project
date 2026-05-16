#ifndef ADD_BOOK_COMMAND_HPP
#define ADD_BOOK_COMMAND_HPP

#include <string>
#include <optional>
#include <cstdint>

namespace Actions {

struct AddBookCommand {
    std::string m_title;
    std::string m_author;
    std::optional<uint16_t> m_publicationYear {};
};

} // namespace Actions

#endif // ADD_BOOK_COMMAND_HPP
