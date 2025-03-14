# Define a list of source files for quartz
set(QUARTZ_SOURCES
    src/quartz.cpp
	src/tokenizer/tokenizer.cpp
	src/utils/fileUtils.cpp
	src/logging/logging.cpp
	src/parser/parser.cpp
)

set(QUARTZ_HEADERS
	include/quartz/quartz.hpp
	include/quartz/tokenizer/tokenizer.hpp
	include/quartz/tokenizer/tokens.hpp
	include/quartz/utils/fileUtils.hpp
	include/quartz/logging/logging.hpp
	include/quartz/parser/abstractSyntaxTree.hpp
	include/quartz/parser/parser.hpp
)

# Define the precompiled header for quartz
set(QUARTZ_PCH_FILE ${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp)

# Define the quartz library
add_library(quartz STATIC ${QUARTZ_SOURCES} ${QUARTZ_HEADERS})

# Specify include directories for quartz
target_include_directories(quartz PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}   # Source directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/quartz  # Include directory
)

# Set the precompiled header for quartz
target_precompile_headers(quartz PRIVATE ${QUARTZ_PCH_FILE})

set_target_properties(quartz PROPERTIES
	OUTPUT_NAME quartz
)
