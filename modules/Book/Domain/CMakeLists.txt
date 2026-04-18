# =========================================================
# DOMAIN
# =========================================================

add_library(BookDomain
    src/Book.cpp
    src/BookId.cpp
    src/BookFilters.cpp
)

target_include_directories(BookDomain
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_TESTING)
    add_subdirectory(unittests/)
endif()
