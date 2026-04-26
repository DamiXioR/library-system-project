# --- Program ---
add_executable(MyProgram main.cpp)

target_compile_options(MyProgram PRIVATE
    -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=.
)

target_include_directories(MyProgram PRIVATE
    ${CMAKE_SOURCE_DIR}/modules/Utilities/Logger/include
)

target_link_libraries(MyProgram PRIVATE
    BookDomain
)
