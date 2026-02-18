# --- Data module (library) ---

add_library(DataModule SHARED
    src/Data.cpp
)
target_include_directories(DataModule PUBLIC
    include
)

option(BUILD_DATAMODULE_TESTS "Build DataModule tests" ON)

if(BUILD_TESTING AND BUILD_DATAMODULE_TESTS)
    add_subdirectory(unittests)
endif()
