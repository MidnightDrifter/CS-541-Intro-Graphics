////////////////////////////////////////////////////////////////////////
// A small library of object shapes (ground plane, sphere, and the
// famous Utah teapot), each created as a batch of Vertex Buffer
// Objects collected under a Vertex Array Object umbrella.  This is
// the latest and most efficient way to get geometry into the OpenGL
// graphics pipeline.
//
// Each vertex is specified as four attributes which are made
// available in a vertes shader in the following attribute slots.
//
// position,        vec4,   attribute #0
// normal,          vec3,   attribute #1
// texture coord,   vec3,   attribute #2
// tangent,         vec3,   attribute #3
//
// An instance of any of these shapes is create with a single call:
//    unsigned int obj = CreateSphere(divisions, &quadCount);
// and drawn by:
//    glBindVertexArray(obj);
//    glDrawElements(GL_QUADS, quadCount, GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//
// Copyright 2013 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#ifndef _MODELS
#define _MODELS

#include "transform.h"
#include "rply.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;

#include <vector>

class Model
{
public:

    Model() :animate(false) {}
    virtual ~Model() {}

    // Data arrays
    std::vector<vec4> Pnt;
    std::vector<vec3> Nrm;
    std::vector<vec2> Tex;
    std::vector<vec3> Tan;

    // Lighting information
    vec3 diffuseColor, specularColor;
    float shininess;

    // Geometry defined by indices into data arrays
    std::vector<ivec4> Quad;
    std::vector<ivec3> Tri;
    unsigned int count;
    unsigned int shape;

    // Defined by SetTransform by scanning data arrays
    vec3 minP, maxP;
    vec3 center;
    float size;
    MAT4 modelTr;
    bool animate;

    // Defined by MakeVAO when/if sending to OpenGL
    unsigned int vao;





    virtual void ComputeSize();
    virtual void MakeVAO();
    virtual void DrawVAO();
};

class Sphere: public Model
{
public:
    Sphere(const int n);
};

class Teapot: public Model
{
public:
    Teapot(const int n);
};

class Ground: public Model
{
public:
    Ground(const float range, const int n);
};

class Ply: public Model
{
public:
    Ply(const char* name, const bool reverse=false);
    virtual ~Ply() {printf("destruct Ply\n");};
    static int vertex_cb(p_ply_argument argument);
    static int face_cb(p_ply_argument argument);
};

#endif
