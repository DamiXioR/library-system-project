#ifndef REMOVE_BOOK_COMMAND_HPP
#define REMOVE_BOOK_COMMAND_HPP

#include "BookId.hpp"

namespace Actions {

struct RemoveBookCommand {
    BookId m_bookId;
};

} // namespace Actions

#endif // REMOVE_BOOK_COMMAND_HPP
