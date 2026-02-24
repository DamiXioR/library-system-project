# --- Unittests for Application ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(LibraryServiceTests
    ${TEST_SRC_FILES}
)

target_link_libraries(LibraryServiceTests PRIVATE
    GTestDependencies
    LibraryService
)

add_test(
    NAME LibraryServiceTests
    COMMAND LibraryServiceTests
)
set_tests_properties(LibraryServiceTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
