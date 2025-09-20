cmake_minimum_required(VERSION 3.16)
project(SecureWiper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the core library
add_subdirectory(src/core)

# Add the CLI test application
add_subdirectory(app/cli)