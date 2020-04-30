#ifndef _MESH_H_
#define _MESH_H_
#include <memory>
#include <string>
#include <vector>
#include <glad/glad.h>

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "stb_image.h"


class Mesh {
 public:
  struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
  };

  static std::unique_ptr<Mesh> Create(const std::vector<Vertex>& vertices,
      const std::vector<unsigned int>& indices);

  inline int id() const {return vao_; }
  ~Mesh();
 private:
  unsigned int vao_;
  unsigned int vbo_;
  unsigned int ebo_;

  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
};


#endif //_MESH_H_
