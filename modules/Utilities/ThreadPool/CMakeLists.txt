# --- Utilities module (ThreadPool) ---

add_library(ThreadPool INTERFACE)

target_include_directories(ThreadPool
    INTERFACE
        ${CMAKE_SOURCE_DIR}/modules/Utilities/Logger/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_TESTING)
    add_subdirectory(unittests)
endif()
