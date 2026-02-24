# --- Unittests for Repository ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(RepositoryTests
    ${TEST_SRC_FILES}
)

target_link_libraries(RepositoryTests PRIVATE
    GTestDependencies
    Repository
    TestUtils
)

add_test(
    NAME RepositoryTests
    COMMAND RepositoryTests
)
set_tests_properties(RepositoryTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
