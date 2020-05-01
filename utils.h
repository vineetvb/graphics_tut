#ifndef CMAKE_BUILD_DEBUG_UTILS_H_
#define CMAKE_BUILD_DEBUG_UTILS_H_
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct DestroyglfwWindow {
  void operator()(GLFWwindow* ptr) {
    glfwDestroyWindow(ptr);
  }
};

void processInput(GLFWwindow* window) ;
std::unique_ptr<GLFWwindow, DestroyglfwWindow> CreateWindow();

#endif //CMAKE_BUILD_DEBUG_UTILS_H_
