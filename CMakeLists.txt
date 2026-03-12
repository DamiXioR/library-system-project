cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyProgram)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(vendor/googletest)

    add_library(GTestDependencies INTERFACE)

    target_link_libraries(GTestDependencies INTERFACE
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
    )
endif()

add_subdirectory(app)
add_subdirectory(modules/Domain)
add_subdirectory(modules/Repository)
add_subdirectory(modules/Application)
add_subdirectory(modules/Utilities)

if(BUILD_TESTING)
    add_subdirectory(modules/TestUtils)
endif()
