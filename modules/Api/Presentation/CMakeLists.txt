# =========================================================
# PRESENTATION - DISPATCHER
# =========================================================

add_library(ApiPresentation
    src/Dispatcher.cpp
)

target_include_directories(ApiPresentation
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/vendor/nlohmann_json
)

# target_link_libraries(ApiPresentation
#     PUBLIC
#         BookDomain
# )

if(BUILD_TESTING)
    add_subdirectory(unittests/)
endif()