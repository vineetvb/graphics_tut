

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ext.hpp>

#include "shader.h"
#include "mesh.h"
#include "utils.h"

int main() {

  // Create Windowing Object, includes current OpenGL context binding.
  auto window = CreateWindow();
  glViewport(/*lower left corner*/0, 0, /*width x height*/800, 600);

  Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

  std::vector<Mesh::Vertex> face = {
      {glm::vec4{0.0f, 0.0f, 0.0f, 1.0f},
       glm::vec3{1.0f, 0.0f, 0.0f},
       glm::vec2{1.0f, 1.0f}},
      {glm::vec4{0.5f, 0.0f, 0.0f, 1.0f},
       glm::vec3{0.0f, 1.0f, 0.0f},
       glm::vec2{1.0f, 0.0f}},   // bottom right
      {glm::vec4{0.5f, 0.5f, 0.0f, 1.0f},
       glm::vec3{0.0f, 0.0f, 1.0f},
       glm::vec2{0.0f, 0.0f}},   // bottom left
      {glm::vec4{0.0f, 0.5f, 0.0f, 1.0f},
       glm::vec3{1.0f, 1.0f, 0.0f},
       glm::vec2{0.0f, 1.0f}}    // top left
  };
  std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

  auto face_mesh = Mesh::Create(face, indices);
  
  if (!face_mesh->AllocateGLBuffers()) {
    std::cerr << "Unable to allocate GL buffers" << std::endl;
  }


  shader.Use();
  int camera_matrix_attrib_location = glGetUniformLocation(shader.ID, "camera");
  glm::mat4 camera = glm::mat4(1.0);

  while (!glfwWindowShouldClose(window.get())) {
    processInput(window.get());
    glUniformMatrix4fv(camera_matrix_attrib_location,
                       1,
                       GL_FALSE,
                       glm::value_ptr(camera));
    shader.Draw(face_mesh.get());
    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}