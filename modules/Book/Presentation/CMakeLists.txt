# =========================================================
# INTERFACE - JSON PARSER
# =========================================================

add_library(BookInterface
    src/BookJsonParser.cpp
)

target_include_directories(BookInterface
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/vendor/nlohmann_json
)

# target_link_libraries(BookInterface
#     PUBLIC
#         BookDomain
# )

if(BUILD_TESTING)
    add_subdirectory(unittests/)
endif()