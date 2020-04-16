cmake_minimum_required(VERSION 3.9)
project(graphics_tut)

set(CMAKE_CXX_STANDARD 17)

add_executable(graphics_tut main.cpp glad/src/glad.c)

find_package(glfw3 3.3 REQUIRED)
find_package(PkgConfig REQUIRED)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(glad/include)

target_link_libraries(graphics_tut glfw ${GLFW_LIBRARIES}  ${CMAKE_DL_LIBS})
