# --- Repository module (library) ---

add_library(Repository SHARED
    src/BookRepository.cpp
    src/MultiIndexedContainer.cpp
    src/FilterMultiIndexedContainer.cpp
)

target_include_directories(Repository PUBLIC
    include
)

target_link_libraries(Repository PUBLIC
    Domain
)

option(BUILD_REPOSITORY_TESTS "Build Repository tests" ON)

if(BUILD_TESTING AND BUILD_REPOSITORY_TESTS)
    add_subdirectory(unittests)
endif()
