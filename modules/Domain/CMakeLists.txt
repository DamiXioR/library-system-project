# --- Domain module (library) ---

add_library(Domain SHARED
    src/Book.cpp
    src/BookId.cpp
)

target_include_directories(Domain PUBLIC
    include
)

option(BUILD_DOMAIN_TESTS "Build Domain tests" ON)

if(BUILD_TESTING AND BUILD_DOMAIN_TESTS)
    add_subdirectory(unittests)
endif()
