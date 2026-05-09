# --- Utilities module (Logger) ---

add_library(Logger INTERFACE)

target_include_directories(Logger
    INTERFACE
        ${CMAKE_SOURCE_DIR}/modules/Utilities/EnvVarFetcher/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_TESTING)
    add_subdirectory(unittests)
endif()
