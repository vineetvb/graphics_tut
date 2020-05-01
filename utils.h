#ifndef CMAKE_BUILD_DEBUG_UTILS_H_
#define CMAKE_BUILD_DEBUG_UTILS_H_
#include <memory>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ext.hpp>
#include "mesh.h"

struct DestroyglfwWindow {
  void operator()(GLFWwindow* ptr) {
    glfwDestroyWindow(ptr);
  }
};

void processInput(GLFWwindow* window) ;
std::unique_ptr<GLFWwindow, DestroyglfwWindow> CreateWindow();


//Printing utils
void Print(const Mesh::Vertex& v);
void Print(const std::vector<Mesh::Vertex>& vertices);
void Print(const Mesh& mesh);
void Print(glm::mat4 m);
void Print(glm::vec4 m);

#endif //CMAKE_BUILD_DEBUG_UTILS_H_
