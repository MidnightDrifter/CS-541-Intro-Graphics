///////////////////////////////////////////////////////////////////////
// A slight encapsulation of an OpenGL texture. This contains a method
// to read an image file into a texture, and methods to bind a texture
// to a shader for use, and unbind when done.
////////////////////////////////////////////////////////////////////////

#ifndef _TEXTURE_
#define _TEXTURE_

class Texture
{
 public:
    int textureId;
    
    Texture() :textureId(0) {};
    void Read(const std::string &filename);
    void Bind(const int unit);
    void Unbind();
};

#endif
