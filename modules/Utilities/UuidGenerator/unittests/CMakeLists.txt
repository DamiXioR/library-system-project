# --- Unittests for Utilities (UuidGenerator) ---

get_target_property(INC UuidGenerator INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "UuidGenerator includes: ${INC}")

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(UuidGeneratorTests
    ${TEST_SRC_FILES}
)

target_link_libraries(UuidGeneratorTests PRIVATE
    GTestDependencies
    UuidGenerator
)

add_test(
    NAME UuidGeneratorTests
    COMMAND UuidGeneratorTests
)

set_tests_properties(UuidGeneratorTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
