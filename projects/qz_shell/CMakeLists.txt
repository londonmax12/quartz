# Define a list of source files for qz_shell executable
set(QZ_SHELL_SOURCES
    src/qz_shell.cpp
)

# Define the qz_shell executable
add_executable(qz_shell ${QZ_SHELL_SOURCES})

# Specify include directories for qz_shell
target_include_directories(qz_shell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../quartz/include)

target_link_libraries(qz_shell PRIVATE quartz)

add_custom_command(TARGET qz_shell POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:quartz> $<TARGET_FILE_DIR:qz_shell>)