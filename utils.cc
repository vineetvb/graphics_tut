#include "utils.h"

#include <iostream>

/* Utilities related to GLFW windows and such.
 *
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

std::unique_ptr<GLFWwindow, DestroyglfwWindow> CreateWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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