#include "texture.h"

#include <string>

#include <glad/glad.h>
#include "stb_image.h"

Texture::Texture(const std::string& image_path) {

  // Load raw image data.
  int width, height, nrChannels;
  unsigned char
      * data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);

  // Generate texture
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

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

  stbi_image_free(data);

}


