#include "texture.h"

#include <string>

#include <glad/glad.h>
#include "stb_image.h"

Texture::Texture(const unsigned char* data, int width, int height, int channels) {
  // Generate texture
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);
  // bind texture data
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGB,
               width,
               height,
               0,
               GL_RGB,
               GL_UNSIGNED_BYTE,
               data);
  glGenerateMipmap(GL_TEXTURE_2D);
}


