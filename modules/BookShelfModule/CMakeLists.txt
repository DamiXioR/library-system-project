# --- BookShelfModule module (library) ---

add_library(BookShelfModule SHARED
    src/Book.cpp
    src/BookId.cpp
)

target_include_directories(BookShelfModule PUBLIC
    include
)

option(BUILD_BOOKSHELFMODULE_TESTS "Build BookShelfModule tests" ON)

if(BUILD_TESTING AND BUILD_BOOKSHELFMODULE_TESTS)
    add_subdirectory(unittests)
endif()
