#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <memory>
#include <string>
#include <glad/glad.h>

class Texture {
 public:
  static std::unique_ptr<Texture> CreateFromImage(const std::string& image_path,
                                                  int texture_unit_id);
  Texture(const unsigned char* data,
          int width,
          int height,
          int texture_unit_id,
          unsigned int texture_type = GL_RGB);
  inline int Handle() const { return texture_handle_; };
  void Texture::Activate() const;
 private:
  unsigned int texture_handle_;
  unsigned int texture_unit_id_;
};

#endif //_TEXTURE_H_
