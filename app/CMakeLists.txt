# --- Program ---
add_executable(MyProgram main.cpp)

target_link_libraries(MyProgram PRIVATE
    DataModule
    ExampleModule
)
