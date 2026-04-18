# --- Unittests for Application ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(BookApplicationTests
    ${TEST_SRC_FILES}
)

target_link_libraries(BookApplicationTests
    PRIVATE
        GTestDependencies
        TestUtils
        BookApplication
        BookInfrastructure_InMemory
)

add_test(
    NAME BookApplicationTests
    COMMAND BookApplicationTests
)
set_tests_properties(BookApplicationTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
