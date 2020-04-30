#include "mesh.h"

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
  unsigned int texAttribLocation = 2;


  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_);
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
                        (void*) (sizeof(glm::vec3)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo_);
  glBindVertexArray(0);
  return std::move(mesh);
}

bool Mesh::SetTextureFromImage(const std::string& image_path) {
   // Load raw image data.
  int width, height, nrChannels;
  unsigned char
      * data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    texture_ = std::make_unique<Texture>(data, width, height, nrChannels);
    stbi_image_free(data);
  }
  else {
    std::cerr << "Unable to load image file " << image_path << std::endl;
    return false;
  }
  return true;
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &ebo_);
}