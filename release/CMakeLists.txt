cmake_minimum_required(VERSION 3.23)

project(MyProgram)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Data module (library) ---
add_library(DataModule SHARED
    DataModule/src/Data.cpp
)
target_include_directories(DataModule PUBLIC
    DataModule/include
)

# --- Program ---
add_executable(MyProgram main.cpp)

target_link_libraries(MyProgram PRIVATE
    DataModule
)

# --- Tests  ---
enable_testing()

file(GLOB_RECURSE TEST_SRC_FILES "DataModule/unittests/*.cpp")

add_executable(MyTests
    ${TEST_SRC_FILES}
)

target_link_libraries(MyTests PRIVATE
    DataModule
)

# > GTest
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