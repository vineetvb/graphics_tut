#include "mesh.h"

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ext/matrix_transform.hpp>
#include <algorithm>
#include "glm/glm.hpp"

std::unique_ptr<Mesh> Mesh::Create(const std::vector<Vertex>& vertices,
                                   const std::vector<unsigned int>& indices) {
  auto mesh = std::make_unique<Mesh>();
  mesh->vertices_.clear();
  mesh->vertices_ = vertices;
  mesh->indices_.clear();
  mesh->indices_ = indices;
  return std::move(mesh);
}

bool Mesh::AllocateGLBuffers() {
   glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER,
               vertices_.size() * sizeof(Vertex),
               vertices_.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices_.size() * sizeof(unsigned int),
               indices_.data(),
               GL_STATIC_DRAW);

  //unsigned int vao;
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  unsigned int positionAttribLocation = 0;
  unsigned int colorAttribLocation = 1;
  unsigned int texAttribLocation = 2;

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glEnableVertexAttribArray(positionAttribLocation);
  glEnableVertexAttribArray(colorAttribLocation);
  glEnableVertexAttribArray(texAttribLocation);

  glVertexAttribPointer(positionAttribLocation,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Mesh::Vertex),
                        (void*) 0);

  glVertexAttribPointer(colorAttribLocation,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Mesh::Vertex),
                        (void*) (sizeof(glm::vec4)));

  glVertexAttribPointer(texAttribLocation,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Mesh::Vertex),
                        (void*) (sizeof(glm::vec4) + sizeof(glm::vec3)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBindVertexArray(0);
  return true;
}

Mesh::~Mesh() {
}

bool Mesh::Attach(std::unique_ptr<Mesh> another) {
  // append vertices
  vertices_.insert(vertices_.end(), another->vertices_.begin(), another->vertices_.end());
  // find largest element index current mesh
  auto largest_index = vertices_.size();
  for(int i = 0; i < another->indices_.size(); ++i){
    another->indices_[i] += largest_index;
  }
  indices_.insert(indices_.end(), another->indices_.begin(), another->indices_.end());

  // dealloc buffers if any;
}

void Mesh::Translate(const glm::vec3& tvec) {
  glm::mat4 tf = glm::translate(glm::mat4(1.0f), tvec);
  for (auto& v: vertices_) {
    v.Position = tf * v.Position;
  }
}

void Mesh::RotateX(float rx) {
  glm::mat4 tf = glm::rotate(glm::mat4(1.0f), rx, glm::vec3(1.0, 0.0, 0.0));
  for (auto& v: vertices_) {
    v.Position = tf * v.Position;
  }
}

void Mesh::RotateY(float ry) {
  glm::mat4 tf = glm::rotate(glm::mat4(1.0f), ry, glm::vec3(0.0, 1.0, 0.0));
  for (auto& v: vertices_) {
    v.Position = tf * v.Position;
  }
}

void Mesh::RotateZ(float rz) {
  glm::mat4 tf = glm::rotate(glm::mat4(1.0f), rz, glm::vec3(0.0, 0.0, 1.0));
  for (auto& v: vertices_) {
    v.Position = tf * v.Position;
  }
}