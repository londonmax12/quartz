# Define a list of source files for qz_interpreter executable
set(QZ_INTERPRETER_SOURCES
    src/interpreter.cpp
	src/interpreter.hpp
	src/main.cpp
	src/strategy/strategy.cpp
	src/strategy/strategy.hpp
)

# Define the qz_interpreter executable
add_executable(qz_interpreter ${QZ_INTERPRETER_SOURCES})

# Specify include directories for qz_interpreter
target_include_directories(qz_interpreter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../quartz/include)

target_link_libraries(qz_interpreter PRIVATE quartz)

add_custom_command(TARGET qz_interpreter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:quartz> $<TARGET_FILE_DIR:qz_interpreter>)