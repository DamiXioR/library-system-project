# --- Unittests for Utilities (EnvVarFetcher) ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(EnvVarFetcherTests
    ${TEST_SRC_FILES}
)

target_include_directories(EnvVarFetcherTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks
)

target_link_libraries(EnvVarFetcherTests PRIVATE
    GTestDependencies
    EnvVarFetcher
)

add_test(
    NAME EnvVarFetcherTests
    COMMAND EnvVarFetcherTests
)

set_tests_properties(EnvVarFetcherTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
