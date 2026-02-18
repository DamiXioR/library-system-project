# --- Example module (library) ---

add_library(ExampleModule SHARED
    src/Example.cpp
)
target_include_directories(ExampleModule PUBLIC
    include
)

option(BUILD_EXAMPLEMODULE_TESTS "Build ExampleModule tests" ON)

if(BUILD_TESTING AND BUILD_EXAMPLEMODULE_TESTS)
    add_subdirectory(unittests)
endif()
