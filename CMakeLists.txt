cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyProgram)

include(CTest)

add_subdirectory(app)
add_subdirectory(modules/BookShelfModule)
add_subdirectory(modules/UtilitiesModule)
add_subdirectory(modules/ExampleModule)

if(BUILD_TESTING)
    add_subdirectory(vendor/googletest)
endif()
