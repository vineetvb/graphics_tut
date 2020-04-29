cmake_minimum_required(VERSION 3.9)
project(graphics_tut)

set(CMAKE_CXX_STANDARD 17)

add_executable(graphics_tut main.cpp glad/src/glad.c stb_image.h )
add_executable(texture_main texture_basic.cpp glad/src/glad.c stb_image.h)
add_executable(transparent_textures transparent_textures.cc glad/src/glad.c stb_image.h)
add_executable(minimal minimal.cc glad/src/glad.c stb_image.h)


find_package(glfw3 3.3 REQUIRED)
find_package(PkgConfig REQUIRED)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(glad/include)
include_directories(glm)

target_link_libraries(graphics_tut glfw ${GLFW_LIBRARIES}  ${CMAKE_DL_LIBS})
target_link_libraries(texture_main glfw ${GLFW_LIBRARIES}  ${CMAKE_DL_LIBS})
target_link_libraries(transparent_textures glfw ${GLFW_LIBRARIES}  ${CMAKE_DL_LIBS})
target_link_libraries(minimal glfw ${GLFW_LIBRARIES}  ${CMAKE_DL_LIBS})
