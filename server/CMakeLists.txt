cmake_minimum_required(VERSION 3.10)
project(MathServer)

set(CMAKE_C_STANDARD 99)

# Define variables for include directories and source files
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCE_FILES
    server.c
    addition.c
    subtraction.c
    multiplication.c
    division.c
)

# Add server executable
add_executable(server ${SOURCE_FILES})

# Include directories
target_include_directories(server PRIVATE ${INCLUDE_DIRS})
