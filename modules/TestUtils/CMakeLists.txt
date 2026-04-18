# --- TestUtils module (library) ---

add_library(TestUtils INTERFACE)

target_include_directories(TestUtils INTERFACE
    ${CMAKE_SOURCE_DIR}/modules/Utilities/include
)

target_include_directories(TestUtils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
)

target_link_libraries(TestUtils INTERFACE
    BookDomain
)
