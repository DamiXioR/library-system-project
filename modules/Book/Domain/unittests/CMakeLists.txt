# --- Unittests for Domain ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")
  
add_executable(BookDomainTests
   ${TEST_SRC_FILES}
)

target_link_libraries(BookDomainTests PRIVATE
    GTestDependencies
    BookDomain
)

add_test(
    NAME BookDomainTests
    COMMAND BookDomainTests
)
set_tests_properties(BookDomainTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
