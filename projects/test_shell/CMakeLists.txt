# Define a list of source files for test_shell executable
set(TEST_SHELL_SOURCES
    src/test_shell.cpp
)

# Define the test_shell executable
add_executable(test_shell_app ${TEST_SHELL_SOURCES})

# Specify include directories for test_shell
target_include_directories(test_shell_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../quartz/include)

target_link_libraries(test_shell_app PRIVATE quartz)

add_custom_command(TARGET test_shell_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:quartz> $<TARGET_FILE_DIR:test_shell_app>)