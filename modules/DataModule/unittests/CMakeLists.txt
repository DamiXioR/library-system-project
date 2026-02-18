# --- Unittests fro Data module ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(DataModuleTests
    ${TEST_SRC_FILES}
)

target_link_libraries(DataModuleTests PRIVATE
    DataModule
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)

add_test(
    NAME DataModuleTests
    COMMAND DataModuleTests
)
set_tests_properties(DataModuleTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
