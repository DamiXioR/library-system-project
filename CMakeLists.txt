cmake_minimum_required(VERSION 3.23)

project(LibrarySystem)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

# =========================
# DEPENDENCIES
# =========================
if(BUILD_TESTING)
    set(BUILD_SHARED_LIBS ON)
    add_subdirectory(vendor/googletest)

    add_library(GTestDependencies INTERFACE)
    target_link_libraries(GTestDependencies INTERFACE
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
    )
    set(BUILD_SHARED_LIBS OFF)
endif()

# =========================
# APP
# =========================
add_subdirectory(app)

# =========================
# MODULES (FEATURES)
# =========================
add_subdirectory(modules/Book)
# add_subdirectory(modules/User)
add_subdirectory(modules/Utilities/UuidGenerator)
add_subdirectory(modules/Utilities/Logger)

if(BUILD_TESTING)
    add_subdirectory(modules/TestUtils)
endif()
