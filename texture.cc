#include "texture.h"

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const unsigned char* data,
                 int width,
                 int height,
                 int texture_unit_id,
                 unsigned int texture_type)
    : texture_unit_id_(texture_unit_id) {
  // Generate texture
  glGenTextures(1, &texture_handle_);

  Activate();

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

std::unique_ptr<Texture> Texture::CreateFromImage(const std::string& image_path,
                                                  int texture_unit_id) {
  int width, height, nrChannels;
  unsigned char
      * data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    unsigned int texture_type = GL_RGB;
    if (nrChannels > 3)
      texture_type = GL_RGBA;
    auto texture =
        std::make_unique<Texture>(data,
                                  width,
                                  height,
                                  texture_unit_id,
                                  texture_type);
    stbi_image_free(data);
    return std::move(texture);
  } else {
    std::cerr << "Unable to load image file " << image_path << std::endl;
    return nullptr;
  }
}

void Texture::Activate() const {
  glActiveTexture(GL_TEXTURE0 + texture_unit_id_);
  glBindTexture(GL_TEXTURE_2D, texture_handle_);
}

void Texture::SetTextureUnitId(unsigned int new_id){
  texture_unit_id_ = new_id;
}

