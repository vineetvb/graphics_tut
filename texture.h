#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <string>
#include <glad/glad.h>

class Texture {
 public:
  Texture(const unsigned char* data,
          int width,
          int height,
          int texture_unit_id,
          unsigned int texture_type = GL_RGB);
  inline int Handle() const { return texture_; };
 private:
  unsigned int texture_;
};

#endif //_TEXTURE_H_
