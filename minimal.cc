#include <chrono>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <cmath>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

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

void CreatePolygon(  unsigned int& vao){

  // set up vertex data (and buffer(s)) and configure vertex attributes
  float vertices[] = {
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
  };
    std::cout << "SOV " << sizeof(vertices) << std::endl;

  unsigned int indices[] = {
      0, 1, 3,  // first Triangle
  };
   std::cout << "SOV " << sizeof(indices) << std::endl;

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 8 * 4, vertices, GL_STATIC_DRAW);

  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               3 * 4,
               indices,
               GL_STATIC_DRAW);


  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  unsigned int
      positionAttribLocation = 0;
  // VertexAttribPointer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(positionAttribLocation);
  glVertexAttribPointer(positionAttribLocation,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        8 * sizeof(float),
                        (void*) 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBindVertexArray(0);
}


int main() {
  auto window = CreateWindow();
  glViewport(/*lower left corner*/0, 0, /*width x height*/800, 600);

  Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  float vertices[] = {
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
  };
  unsigned int indices[] = {
      0, 1, 3,  // first Triangle
  };

  unsigned int vao;
  CreatePolygon(vao);


  while (!glfwWindowShouldClose(window.get())) {
    processInput(window.get());

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    shader.use();
    glBindVertexArray(vao);

    // seeing as we only have a single vao there's no need to bind it every
    // time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    glfwSwapBuffers(window.get());
    glfwPollEvents();

  }

  // optional: de-allocate all resources once they've outlived their purpose:
  glDeleteVertexArrays(1, &vao);
//  glDeleteBuffers(1, &vbo);
//  glDeleteBuffers(1, &ebo);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  glfwTerminate();
  return 0;
}