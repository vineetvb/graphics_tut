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

  std::vector<Mesh::Vertex> rectangle = {
      {glm::vec4{0.0f, 0.0f, 0.0f, 1.0f},
       glm::vec3{1.0f, 0.0f, 0.0f},
       glm::vec2{1.0f, 1.0f}},
      {glm::vec4{0.5f, -0.5f, 0.0f, 1.0f},
       glm::vec3{0.0f, 1.0f, 0.0f},
       glm::vec2{1.0f, 0.0f}},   // bottom right
      {glm::vec4{-0.5f, -0.5f, 0.0f, 1.0f},
       glm::vec3{0.0f, 0.0f, 1.0f},
       glm::vec2{0.0f, 0.0f}},   // bottom left
      {glm::vec4{0.0f, 0.5f, 0.0f, 1.0f},
       glm::vec3{1.0f, 1.0f, 0.0f},

       glm::vec2{0.0f, 1.0f}}    // top left
  };
  std::vector<unsigned int> indices = {0, 1, 2, 0, 1, 3};

  auto mesh = Mesh::Create(rectangle, indices);

  if (!mesh->AllocateGLBuffers()) {
    std::cerr << "Unable to allocate GL buffers" << std::endl;
  }

  std::vector<std::unique_ptr<Texture>> textures;

  textures.push_back(Texture::CreateFromImage("wall.jpg", 0));
  textures.push_back(Texture::CreateFromImage("window.png", 1));
  textures.push_back(Texture::CreateFromImage("face.jpg", 0));
  textures.push_back(Texture::CreateFromImage("bike.jpg", 1));

  shader.Use();
  shader.SetUniformInt("texture0_sampler", 0);
  shader.SetUniformInt("texture1_sampler", 1);

  int camera_matrix_attrib_location = glGetUniformLocation(shader.ID, "camera");
  glm::mat4 camera = glm::mat4(1.0);

  // Simple parameter to make the triangle perform SHM.
  float s = 0.0;

  textures[2]->Activate();
  textures[1]->Activate();

  textures[0]->SetTextureUnitId(1);
  textures[0]->Activate();

  while (!glfwWindowShouldClose(window.get())) {
    processInput(window.get());
    glUniformMatrix4fv(camera_matrix_attrib_location,
                       1,
                       GL_FALSE,
                       glm::value_ptr(camera));
    // triangle top is performing SHM.
    camera[3][1] = 0.5 * std::cos(s);  // xtrans
    s += 0.01;

    shader.Draw(mesh.get());
    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}