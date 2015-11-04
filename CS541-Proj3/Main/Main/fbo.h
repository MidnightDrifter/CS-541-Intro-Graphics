///////////////////////////////////////////////////////////////////////
// A slight encapsulation of a Frame Buffer Object (i'e' Render
// Target) and its associated texture.  When the FBO is "Bound", the
// output of the graphics pipeline is captured into the texture.  When
// it is "Unbound", the texture is available for use as any normal
// texture.
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
#ifndef FBO_H
#define FBO_H
class FBO {
public:
    unsigned int fbo;

    unsigned int texture;
    int width, height;  // Size of the texture.

    void CreateFBO(const int w, const int h);
    void Bind();
    void Unbind();
};
#endif