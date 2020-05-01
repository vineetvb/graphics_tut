#include "texture.h"

#include <string>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const unsigned char* data,
                 int width,
                 int height,
                 int texture_unit_id,
                 unsigned int texture_type) {
  // Generate texture
  glGenTextures(1, &texture_);
  glActiveTexture(GL_TEXTURE0 + texture_unit_id);
  glBindTexture(GL_TEXTURE_2D, texture_);

  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S,
                  GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // bind texture data
  glTexImage2D(GL_TEXTURE_2D,
               0,
               texture_type,
               width,
               height,
               0,
               texture_type,
               GL_UNSIGNED_BYTE,
               data);
  glGenerateMipmap(GL_TEXTURE_2D);
}


