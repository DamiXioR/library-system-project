# --- Unittests for ExampleModule ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(ExampleModuleTests
    ${TEST_SRC_FILES}
)

target_link_libraries(ExampleModuleTests PRIVATE
    ExampleModule
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)

add_test(
    NAME ExampleModuleTests
    COMMAND ExampleModuleTests
)
set_tests_properties(ExampleModuleTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
