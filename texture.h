#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <string>

class Texture{
 public:
  Texture(const unsigned char* data, int width, int height, int channels);
 private:
    unsigned int texture_;

};


#endif //_TEXTURE_H_
