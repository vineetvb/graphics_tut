
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "shader.h"
#include "mesh.h"
#include "utils.h"

int main() {

  // Create Windowing Object, includes current OpenGL context binding.
  auto window = CreateWindow();
  glViewport(/*lower left corner*/0, 0, /*width x height*/800, 600);

  Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

  std::vector<Mesh::Vertex> rectangle = {
      {glm::vec4{0.5f, 0.5f, 0.0f, 1.0f}, glm::vec3{1.0f, 0.0f, 0.0f},
       glm::vec2{1.0f, 1.0f}},
      {glm::vec4{0.5f, -0.5f, 0.0f,1.0f}, glm::vec3{0.0f, 1.0f, 0.0f},
       glm::vec2{1.0f, 0.0f}},   // bottom right
      {glm::vec4{-0.5f, -0.5f, 0.0f,1.0f}, glm::vec3{0.0f, 0.0f, 1.0f},
       glm::vec2{0.0f, 0.0f}},   // bottom left
      {glm::vec4{-0.5f, 0.5f, 0.0f,1.0f}, glm::vec3{1.0f, 1.0f, 0.0f},
       glm::vec2{0.0f, 1.0f}}    // top left
  };
  std::vector<unsigned int> indices = {0, 1, 3, 1, 2, 3};

  auto mesh = Mesh::Create(rectangle, indices);

  if( !mesh->AllocateGLBuffers() ){
    std::cerr << "Unable to allocate GL buffers" << std::endl;
  }

  mesh->SetTextureFromImage("wall.jpg");
  mesh->SetTextureFromImage("window.png", 1);

  shader.Use();
  shader.SetUniformInt("texture0_sampler", 0);
  shader.SetUniformInt("texture1_sampler", 1);

  while (!glfwWindowShouldClose(window.get())) {
    processInput(window.get());
    shader.Draw(mesh.get());
    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}