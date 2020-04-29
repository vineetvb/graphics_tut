
#include <chrono>
#include <iostream>
#include <string>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

/*
 * Create window and manage GLAD init.
 */
struct DestroyglfwWindow {

  void operator()(GLFWwindow* ptr) {
    glfwDestroyWindow(ptr);
  }

};
std::unique_ptr<GLFWwindow, DestroyglfwWindow> CreateWindow() {

  auto window =
      std::unique_ptr<GLFWwindow, DestroyglfwWindow>(glfwCreateWindow(800,
                                                                      600,
                                                                      "OpenGL: Transparent Textures",
                                                                      NULL,
                                                                      NULL)); // ignore last 2 params
  if (window == nullptr) {
    std::cout << "Failed to create Window." << std::endl;
    glfwTerminate();
    throw std::runtime_error("Unable to Create GLFW window.");
  }

  // Tell GLFW to make window to be the main context on the current thread.
  glfwMakeContextCurrent(window.get());
  // Set callback for window resize.
  glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);

  // GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function.
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to init GLAD" << std::endl;
    throw std::runtime_error("Unable to Init GLAD.");
  }

  return std::move(window);
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create Windowing Object.
  auto window = CreateWindow();
  glViewport(/*lower left corner*/0, 0, /*width x height*/800, 600);

  while (!glfwWindowShouldClose(window.get())) {
    processInput(window.get());
    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

  return 0;
}