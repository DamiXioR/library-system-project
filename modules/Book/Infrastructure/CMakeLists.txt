# =========================================================
# INFRASTRUCTURE - IN MEMORY
# =========================================================

add_library(BookInfrastructure_InMemory
    Persistence/InMemory/src/InMemoryBookRepository.cpp
    Persistence/InMemory/src/MultiIndexedContainer.cpp
)

target_include_directories(BookInfrastructure_InMemory
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Persistence/InMemory/include
)

target_link_libraries(BookInfrastructure_InMemory
    PUBLIC
        BookDomain
        BookInfrastructure_QueryAdapters
)

# =========================================================
# INFRASTRUCTURE - QUERY ADAPTERS
# =========================================================

add_library(BookInfrastructure_QueryAdapters
    QueryAdapters/src/BookFilteringForMultiIndexedContainer.cpp
)

target_include_directories(BookInfrastructure_QueryAdapters
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/QueryAdapters/include
)

target_link_libraries(BookInfrastructure_QueryAdapters
    PUBLIC
        BookDomain
        BookInfrastructure_InMemory
)

if(BUILD_TESTING)
    add_subdirectory(unittests/)
endif()
