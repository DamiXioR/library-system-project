# --- Unittests for Utilities (Logger) ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(LoggerTests
    ${TEST_SRC_FILES}
)

target_include_directories(LoggerTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks
)

target_link_libraries(LoggerTests PRIVATE
    GTestDependencies
    Logger
)

add_test(
    NAME LoggerTests
    COMMAND LoggerTests
)

set_tests_properties(LoggerTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
