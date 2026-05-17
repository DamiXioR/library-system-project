# --- Unittests for BookInterface ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")
  
add_executable(BookInterfaceTests
   ${TEST_SRC_FILES}
)

target_link_libraries(BookInterfaceTests PRIVATE
    GTestDependencies
    BookInterface
)

add_test(
    NAME BookInterfaceTests
    COMMAND BookInterfaceTests
)
set_tests_properties(BookInterfaceTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
