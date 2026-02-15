cmake_minimum_required(VERSION 3.23)

project(MyProgram)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Program ---
file(GLOB_RECURSE SRC_FILES "product/src/*.cpp")

add_executable(MyProgram
    main.cpp
    ${SRC_FILES}
)

target_include_directories(MyProgram PRIVATE
    product/include/
)

# --- Tests  ---
enable_testing()

file(GLOB_RECURSE TEST_SRC_FILES "unittests/*.cpp")

add_executable(MyTests
    ${SRC_FILES}
    ${TEST_SRC_FILES}
)

target_include_directories(MyTests PRIVATE
    product/include/
)

# ---- GTest ----
set(GTEST_ROOT  /home/dmx/workspace/good_job_project/googletest)

target_include_directories(MyTests PRIVATE
    ${GTEST_ROOT}/googletest/include
    ${GTEST_ROOT}/googlemock/include
)

target_link_libraries(MyTests PRIVATE
    ${GTEST_ROOT}/build/lib/libgtest.so
    ${GTEST_ROOT}/build/lib/libgtest_main.so
    ${GTEST_ROOT}/build/lib/libgmock.so
    ${GTEST_ROOT}/build/lib/libgmock_main.so
)

add_test(
    NAME MyTests
    COMMAND MyTests
)