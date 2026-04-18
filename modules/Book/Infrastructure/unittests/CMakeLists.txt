# --- Unittests for Book Infrastructure ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(BookInfrastructureTests
    ${TEST_SRC_FILES}
)

target_link_libraries(BookInfrastructureTests
    PRIVATE
        GTestDependencies
        TestUtils
        BookInfrastructure_InMemory
        BookDomain
)

add_test(
    NAME BookInfrastructureTests
    COMMAND BookInfrastructureTests
)
set_tests_properties(BookInfrastructureTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
