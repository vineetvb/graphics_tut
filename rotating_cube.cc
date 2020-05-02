

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ext.hpp>

#include "shader.h"
#include "mesh.h"
#include "utils.h"

constexpr double DEG_TO_RADIANS = 3.14159265 / 180.0;
using std::cout;
using std::endl;

int main() {

  // Create Windowing Object, includes current OpenGL context binding.
  auto window = CreateWindow();
  glViewport(/*lower left corner*/0, 0, /*width x height*/800, 600);

  Shader shader("shaders/perspective_vertices.glsl", "shaders/fragment.glsl");
  float test_z = 0; // must be in range [-1, 1]
  float test_x = 0.73;
  float test_y = 0.93;
  float test_w = 1.0;
  std::vector<Mesh::Vertex> test_face = {
      {glm::vec4{0.0f, 0.0f, test_z, test_w},
       glm::vec3{1.0f, 0.0f, 0.0f},
       glm::vec2{1.0f, 1.0f}},
      {glm::vec4{test_x, 0.0f, test_z, test_w},
       glm::vec3{0.0f, 1.0f, 0.0f},
       glm::vec2{1.0f, 0.0f}},   // bottom right
      {glm::vec4{test_x, test_y, test_z, test_w},
       glm::vec3{0.0f, 0.0f, 1.0f},
       glm::vec2{0.0f, 0.0f}},   // bottom left
      {glm::vec4{0.0f, test_y, test_z, test_w},
       glm::vec3{1.0f, 1.0f, 1.0f},
       glm::vec2{0.0f, 1.0f}}    // top left
  };
//  std::vector<unsigned int> test_indices = {0, 1, 2};
  std::vector<unsigned int> test_indices = {0, 1, 2, 2, 3, 0};

  auto test_mesh = Mesh::Create(test_face, test_indices);
  test_mesh->AllocateGLBuffers();

  shader.Use();
  unsigned int
      local_to_world_attrib_location =
      glGetUniformLocation(shader.ID, "mesh_local_to_world");
  unsigned int
      camera_matrix_attrib_location =
      glGetUniformLocation(shader.ID, "world_to_camera");
  unsigned int
      proj_matrix_attrib_location = glGetUniformLocation(shader.ID, "proj");

  glm::mat4 local2world = glm::mat4(1.0f);
  glUniformMatrix4fv(local_to_world_attrib_location,
                     1,
                     GL_FALSE,
                     glm::value_ptr(local2world));

  glm::mat4 world2camera;
  world2camera = glm::mat4(1.0);
  world2camera = glm::translate(world2camera, glm::vec3(0.0, 0.0, -2.5));
//  Print(world2camera);

  glm::mat4 proj1, proj2;
  proj1 = glm::perspective(1.0, 4.0 / 3.0, 0.1, 1000.0);
  std::cout << "Projection Matrix 1 : " << std::endl;
  Print(proj1);

  proj2 = glm::mat4(1.0f);
  std::cout << "Projection Matrix 2 : " << std::endl;
  Print(proj2);

  glm::mat4 proj = proj1;

  glUniformMatrix4fv(proj_matrix_attrib_location,
                     1,
                     GL_FALSE,
                     glm::value_ptr(proj));
  glUniformMatrix4fv(camera_matrix_attrib_location,
                     1,
                     GL_FALSE,
                     glm::value_ptr(world2camera));

  std::cout << std::endl;
  Print(proj * world2camera);
  auto vertices = test_mesh->GetVertices();
  for (auto& v : vertices) {
    auto v2 = proj * world2camera * v.Position;
    Print(v2);
    Print(v2 / v2[3]);
    cout << endl;
  }

  while (!glfwWindowShouldClose(window.get())) {
//
    local2world =
        glm::rotate(local2world, 0.02f, glm::vec3(1.0f, 0.0f, 0.0f));
  glUniformMatrix4fv(local_to_world_attrib_location,
                     1,
                     GL_FALSE,
                     glm::value_ptr(local2world));


    world2camera =
        glm::rotate(world2camera, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
  glUniformMatrix4fv(camera_matrix_attrib_location,
                     1,
                     GL_FALSE,
                     glm::value_ptr(world2camera));

    processInput(window.get());
    shader.Use();
    shader.Draw(test_mesh.get());
    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

//  glfwTerminate();
  return 0;
}