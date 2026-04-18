# =========================================================
# APPLICATION
# =========================================================

add_library(BookApplication
    src/LibraryService.cpp
    src/BookFiltersCreator.cpp
)

target_include_directories(BookApplication
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(BookApplication
    PUBLIC
        BookDomain
)

if(BUILD_TESTING)
    add_subdirectory(unittests/)
endif()
