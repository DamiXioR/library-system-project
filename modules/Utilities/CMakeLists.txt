# --- Utilities module (library) ---

add_library(Utilities INTERFACE)

target_include_directories(Utilities
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_TESTING)
    add_subdirectory(unittests)
endif()
