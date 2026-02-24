# --- Unittests for Domain ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(DomainTests
    ${TEST_SRC_FILES}
)

target_link_libraries(DomainTests PRIVATE
    GTestDependencies
    Domain
)

add_test(
    NAME DomainTests
    COMMAND DomainTests
)
set_tests_properties(DomainTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
