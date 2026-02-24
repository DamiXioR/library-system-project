# --- Utilities module (library) ---

add_library(Utilities SHARED
    src/ExampleUtility.cpp
)

target_include_directories(Utilities PUBLIC
    include
)

option(BUILD_UTILITIES_TESTS "Build Utilities tests" ON)

if(BUILD_TESTING AND BUILD_UTILITIES_TESTS)
    add_subdirectory(unittests)
endif()
