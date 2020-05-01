#ifndef _MESH_H_
#define _MESH_H_
#include <memory>
#include <string>
#include <vector>
#include <glad/glad.h>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/vec2.hpp"
#include "stb_image.h"
#include "texture.h"

class Mesh {
 public:
  struct Vertex {
    glm::vec4 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
  };

  static std::unique_ptr<Mesh> Create(const std::vector<Vertex>& vertices,
                                      const std::vector<unsigned int>& indices);
  static std::unique_ptr<Mesh> Clone(const Mesh* const other);

//  bool SetTextureFromImage(const std::string& image_path,
//                           int texture_unit_id = 0);
  void ActivateTextureUnit(int i) const;

  bool AllocateGLBuffers();

  bool Attach(std::unique_ptr<Mesh> another);

  inline int id() const { return vao_; }
  const Texture& GetTexture(int i) const {
    return *texture_[i];
  }
  const std::vector<Vertex> GetVertices() const { return vertices_; };
  const std::vector<unsigned int> GetIndices() const { return indices_; };

  // Geometric Transformations
  void Translate(const glm::vec3& tvec);
  // Rotate CCW about +ve Xaxis.
  void RotateX(float rx);
  void RotateY(float ry);
  void RotateZ(float rz);

  ~Mesh();

 private:
  unsigned int vao_;
  unsigned int vbo_;
  unsigned int ebo_;

  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<const Texture*> texture_;
};

#endif //_MESH_H_
