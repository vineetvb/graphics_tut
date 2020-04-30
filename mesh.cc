#include "mesh.h"

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::unique_ptr<Mesh> Mesh::Create(const std::vector<Vertex>& vertices,
                                   const std::vector<unsigned int>& indices) {
  auto mesh = std::make_unique<Mesh>();
  mesh->vertices_.clear();
  mesh->vertices_ = vertices;
  mesh->indices_.clear();
  mesh->indices_ = indices;

  glGenBuffers(1, &mesh->vbo_);
  glGenBuffers(1, &mesh->ebo_);

  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(Vertex),
               vertices.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices.size() * sizeof(unsigned int),
               indices.data(),
               GL_STATIC_DRAW);

  //unsigned int vao;
  glGenVertexArrays(1, &mesh->vao_);
  glBindVertexArray(mesh->vao_);

  unsigned int positionAttribLocation = 0;
  unsigned int colorAttribLocation = 1;

  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_);
  glEnableVertexAttribArray(positionAttribLocation);
  glEnableVertexAttribArray(colorAttribLocation);

  glVertexAttribPointer(positionAttribLocation,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Mesh::Vertex),
                        (void*) 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo_);
  glBindVertexArray(0);
  return std::move(mesh);
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &ebo_);
}