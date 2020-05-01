cmake_minimum_required(VERSION 3.9)
project(graphics_tut)

set(CMAKE_CXX_STANDARD 17)

add_executable(graphics_tut main.cpp )
add_executable(texture_main texture_basic.cpp)
add_executable(transparent_textures transparent_textures.cc)
add_executable(minimal minimal.cc)
add_executable(mesh_play mesh_play.cc)
add_executable(rotating_cube rotating_cube.cc)

add_library(graphics_ds mesh.cc texture.cc utils.cc glad/src/glad.c shader.h stb_image.h)

find_package(glfw3 3.3 REQUIRED)
find_package(PkgConfig REQUIRED)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(glad/include)
include_directories(glm)

target_link_libraries(graphics_tut glfw ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS})
target_link_libraries(texture_main glfw ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS})
target_link_libraries(transparent_textures graphics_ds glfw ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS})
target_link_libraries(minimal glfw ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS})
target_link_libraries(mesh_play graphics_ds glfw ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS})
target_link_libraries(rotating_cube graphics_ds glfw ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS})
