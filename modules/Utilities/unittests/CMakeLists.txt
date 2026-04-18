# --- Unittests for Utilities ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(UtilitiesTests
    ${TEST_SRC_FILES}
)

target_link_libraries(UtilitiesTests PRIVATE
    GTestDependencies
    Utilities
)

add_test(
    NAME UtilitiesTests
    COMMAND UtilitiesTests
)

set_tests_properties(UtilitiesTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
