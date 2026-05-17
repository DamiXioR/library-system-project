# --- Unittests for Utilities (ThreadPool) ---

file(GLOB_RECURSE TEST_SRC_FILES "*.cpp")

add_executable(ThreadPoolTests
    ${TEST_SRC_FILES}
)

target_include_directories(ThreadPoolTests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/Utilities/Logger/unittests/mocks
)

target_link_libraries(ThreadPoolTests PRIVATE
    GTestDependencies
    ThreadPool
)

add_test(
    NAME ThreadPoolTests
    COMMAND ThreadPoolTests
)

set_tests_properties(ThreadPoolTests PROPERTIES ENVIRONMENT "GTEST_COLOR=yes")
