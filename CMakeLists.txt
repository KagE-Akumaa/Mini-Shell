cmake_minimum_required(VERSION 3.16)

project(mini_shell LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Use modern C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable stricter compiler warnings (recommended for interviews)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Collect all .cpp files under src/
file(GLOB_RECURSE SOURCES src/*.cpp)

# Create the final executable
add_executable(mini_shell ${SOURCES})

# IMPORTANT: Correct include path
# Since your 'include' folder is directly under project root,
# we expose it to all targets.
target_include_directories(mini_shell PUBLIC ${CMAKE_SOURCE_DIR}/include)
