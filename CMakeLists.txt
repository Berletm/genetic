cmake_minimum_required(VERSION 3.20)

project(genetic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core gl base)
include(${wxWidgets_USE_FILE})

find_package(OpenGL REQUIRED)

add_subdirectory(genetic_algo)
add_subdirectory(app)
