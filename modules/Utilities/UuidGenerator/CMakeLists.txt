# --- Utilities module (UuidGenerator) ---

add_library(UuidGenerator INTERFACE)

target_include_directories(UuidGenerator
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_TESTING)
    add_subdirectory(unittests)
endif()
