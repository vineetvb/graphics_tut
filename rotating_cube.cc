

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ext.hpp>

#include "shader.h"
#include "mesh.h"
#include "utils.h"

constexpr double DEG_TO_RADIANS = 3.14159265 / 180.0;

int main() {

  // Create Windowing Object, includes current OpenGL context binding.
  auto window = CreateWindow();
  glViewport(/*lower left corner*/0, 0, /*width x height*/800, 600);

  Shader shader("shaders/perspective_vertices.glsl", "shaders/fragment.glsl");
  float test_z = -0.5; // must be in range [-1, 1]
  float test_x = 0.73;
  float test_y = 0.93;
  float test_w = 1.0;
  std::vector<Mesh::Vertex> test_face = {
      {glm::vec4{0.0f, 0.0f, test_z, test_w},
       glm::vec3{1.0f, 0.0f, 0.0f},
       glm::vec2{1.0f, 1.0f}},
      {glm::vec4{test_x, 0.0f, test_z + 0.5, test_w},
       glm::vec3{0.0f, 1.0f, 0.0f},
       glm::vec2{1.0f, 0.0f}},   // bottom right
      {glm::vec4{test_x, test_y, test_z, test_w},
       glm::vec3{0.0f, 0.0f, 1.0f},
       glm::vec2{0.0f, 0.0f}},   // bottom left
      {glm::vec4{0.0f, test_y, test_z, test_w},
       glm::vec3{1.0f, 1.0f, 0.0f},
       glm::vec2{0.0f, 1.0f}}    // top left
  };
  std::vector<unsigned int> test_indices = {0, 1, 2};

  auto test_mesh = Mesh::Create(test_face, test_indices);
  test_mesh->AllocateGLBuffers();

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

  std::vector<unsigned int> indices = {0, 1, 2};

  std::vector<std::unique_ptr<Mesh>> faces;
  auto face_bottom = Mesh::Create(face, indices);
  auto face_top =
      face_bottom->Clone(face_bottom.get());
  face_top->Translate(glm::vec3(0.0, 0.0, 1.0));

  auto face_left =
      face_bottom->Clone(face_bottom.get());
  face_left->RotateX(90.0 * DEG_TO_RADIANS);

  auto face_right = Mesh::Clone(face_left.get());
  face_right->Translate(glm::vec3(0.0, 1.0, 0.0));

  faces.push_back(std::move(face_bottom));
//  faces.push_back(std::move(face_top));
//  faces.push_back(std::move(face_left));
//  faces.push_back(std::move(face_right));

  for (auto& face: faces) {
    if (!face->AllocateGLBuffers()) {
      std::cerr << "Unable to allocate GL buffers" << std::endl;
    }
  }

  shader.Use();
  unsigned int
      camera_matrix_attrib_location = glGetUniformLocation(shader.ID, "camera");
  unsigned int
      proj_matrix_attrib_location = glGetUniformLocation(shader.ID, "proj");

  glm::mat4 camera;
  camera = glm::mat4(1.0);
  camera = glm::translate(camera, glm::vec3(0.1, 0.2, 0.1));
//  Print(camera);

  glm::mat4 proj1, proj2;
  proj1 = glm::mat4(1.0f);
  proj2 = glm::mat4(1.0f);

  glm::mat4 proj = proj2;
  proj[3][3] = 0.0f;
  Print(proj);

  glUniformMatrix4fv(proj_matrix_attrib_location,
                     1,
                     GL_FALSE,
                     glm::value_ptr(proj));

  glUniformMatrix4fv(camera_matrix_attrib_location,
                     1,
                     GL_FALSE,
                     glm::value_ptr(camera));

//  Print(proj1 * camera * faces[0]->GetVertices()[0].Position);
//  Print(proj1 * camera * faces[0]->GetVertices()[1].Position);
//  Print(proj1 * camera * faces[0]->GetVertices()[2].Position);
//  Print(proj1 * camera);
//  Print((proj1 * camera) * test_mesh->GetVertices()[0].Position);
//  Print((proj1 * camera) * test_mesh->GetVertices()[1].Position);
//  Print(proj1 * camera * test_mesh->GetVertices()[2].Position);
  std::cout << std::endl;
  Print(proj2 * camera);
  Print(proj2 * camera * test_mesh->GetVertices()[0].Position);
  Print(proj2 * camera * test_mesh->GetVertices()[1].Position);
  Print(proj2 * camera * test_mesh->GetVertices()[2].Position);
//  Print(test_mesh->GetVertices());
//
  while (!glfwWindowShouldClose(window.get())) {
    processInput(window.get());
    shader.Use();


//    shader.Draw(faces);
    shader.Draw(test_mesh.get());
//    shader.Draw(faces[0].get());
    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

//  glfwTerminate();
  return 0;
}