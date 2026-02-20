# --- Unittests for BookShelfModule ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(BookShelfModuleTests
    ${TEST_SRC_FILES}
)

target_include_directories(BookShelfModuleTests PRIVATE
    ${CMAKE_SOURCE_DIR}/modules/UtilitiesModule/include
)

target_link_libraries(BookShelfModuleTests PRIVATE
    BookShelfModule
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)

add_test(
    NAME BookShelfModuleTests
    COMMAND BookShelfModuleTests
)
set_tests_properties(BookShelfModuleTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
