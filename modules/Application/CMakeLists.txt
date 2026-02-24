# --- LibraryService module (library) ---

add_library(LibraryService SHARED
    src/LibraryService.cpp
)

target_include_directories(LibraryService PUBLIC
    include
)

target_link_libraries(LibraryService PUBLIC
    Repository
    TestUtils
)

option(BUILD_APPLICATION_TESTS "Build Application Tests" ON)

if(BUILD_TESTING AND BUILD_APPLICATION_TESTS)
    add_subdirectory(unittests)
endif()
