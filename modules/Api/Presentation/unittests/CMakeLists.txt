# --- Unittests for ApiPresentation ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")
  
add_executable(ApiPresentationTests
   ${TEST_SRC_FILES}
)

target_link_libraries(ApiPresentationTests PRIVATE
    GTestDependencies
    ApiPresentation
)

add_test(
    NAME ApiPresentationTests
    COMMAND ApiPresentationTests
)
set_tests_properties(ApiPresentationTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
