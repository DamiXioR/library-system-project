# --- UtilitiesModule module (library) ---

add_library(UtilitiesModule SHARED
    src/ExampleUtility.cpp
)

target_include_directories(UtilitiesModule PUBLIC
    include
)

option(BUILD_UTILITIESMODULE_TESTS "Build UtilitiesModule tests" ON)

if(BUILD_TESTING AND BUILD_UTILITIESMODULE_TESTS)
    add_subdirectory(unittests)
endif()
