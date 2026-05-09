# --- Utilities module (EnvVarFetcher) ---

add_library(EnvVarFetcher INTERFACE)

target_include_directories(EnvVarFetcher
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_TESTING)
    add_subdirectory(unittests)
endif()
