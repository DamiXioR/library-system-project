cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyProgram)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

add_subdirectory(app)
add_subdirectory(modules/DataModule)
add_subdirectory(modules/ExampleModule)

if(BUILD_TESTING)
    add_subdirectory(vendor/googletest)
endif()
