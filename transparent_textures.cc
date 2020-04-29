
#include <chrono>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
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
 * Create window and manage GLAD init. Binds OpenGL current context to this
 * window.
 */
struct DestroyglfwWindow {

  void operator()(GLFWwindow* ptr) {
    glfwDestroyWindow(ptr);
  }

};
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

void Create2DPolygonAO(const std::vector<float>& vertices,
                      const std::vector<unsigned int>& indices,
                      unsigned int& vao,
                      unsigned int& vbo,
                      unsigned int& ebo) {
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  std::cout << "SOV " << vertices.size() * sizeof(float) << std::endl;
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(float),
               vertices.data(),
               GL_STATIC_DRAW);

   std::cout << "SOI " << indices.size() * sizeof(unsigned int) << std::endl;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices.size() * sizeof(unsigned int),
               indices.data(),
               GL_STATIC_DRAW);

  //unsigned int vao;
  glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

  unsigned int positionAttribLocation = 0;
  unsigned int colorAttribLocation = 1;

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(positionAttribLocation);
  glEnableVertexAttribArray(colorAttribLocation);

  glVertexAttribPointer(positionAttribLocation,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        6 * sizeof(float),
                        (void*) 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // necessary?
  glBindVertexArray(0);
}

int main() {

  // Create Windowing Object, includes current OpenGL context binding.
  auto window = CreateWindow();
  glViewport(/*lower left corner*/0, 0, /*width x height*/800, 600);

  Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  std::vector<float> vertices = {
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
  };
  std::vector<unsigned int> indices = {  // note that we start from 0!
      0, 1, 3,  // first Triangle
  };

  unsigned int polygon1, vbo, ebo;
  Create2DPolygonAO(vertices, indices, polygon1, vbo, ebo);

  shader.use();

  while (!glfwWindowShouldClose(window.get())) {
    processInput(window.get());

    glClearColor(0.2f, 1.0f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    glBindVertexArray(polygon1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &polygon1);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glfwTerminate();
  return 0;
}