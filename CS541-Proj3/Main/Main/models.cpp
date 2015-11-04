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

#include <vector>
#include <fstream>
#include <stdlib.h>
#include <glload/gl_3_3.h>
#include <glload/gl_load.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "math.h"
#include "transform.h"
#include "models.h"
#include "rply.h"

const float PI = 3.14159f;
const float rad = PI/180.0f;
mat4 Identity(1.0);

////////////////////////////////////////////////////////////////////////////////
// Create a Vertex Array Object from (1) a collection of arrays
// containing vertex data and (2) a set of indices inticating quads.
// The arrays must all be the same length and contain respectively,
// the vertex position, normal, texture coordinate, and tangent
// vector.  This is the latest and most efficient way to get geometry
// into the OpenGL graphics pipeline.
unsigned int VaoFromQuads(std::vector<vec4> Pnt,
                          std::vector<vec3> Nrm,
                          std::vector<vec2> Tex,
                          std::vector<vec3> Tan,
                          std::vector<ivec4> Quad)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint Pbuff;
    glGenBuffers(1, &Pbuff);
    glBindBuffer(GL_ARRAY_BUFFER, Pbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*Pnt.size(),
                 &Pnt[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (Nrm.size() > 0) {
        GLuint Nbuff;
        glGenBuffers(1, &Nbuff);
        glBindBuffer(GL_ARRAY_BUFFER, Nbuff);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*Pnt.size(),
                     &Nrm[0][0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); }

    if (Tex.size() > 0) {
        GLuint Tbuff;
        glGenBuffers(1, &Tbuff);
        glBindBuffer(GL_ARRAY_BUFFER, Tbuff);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*Pnt.size(),
                     &Tex[0][0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); }

    if (Tan.size() > 0) {
        GLuint Dbuff;
        glGenBuffers(1, &Dbuff);
        glBindBuffer(GL_ARRAY_BUFFER, Dbuff);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*Pnt.size(),
                     &Tan[0][0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); }

    GLuint Ibuff;
    glGenBuffers(1, &Ibuff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibuff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*4*Quad.size(),
                 &Quad[0][0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    return vao;
}

unsigned int VaoFromTris(std::vector<vec4> Pnt,
                         std::vector<vec3> Nrm,
                         std::vector<vec2> Tex,
                         std::vector<vec3> Tan,
                         std::vector<ivec3> Tri)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint Pbuff;
    glGenBuffers(1, &Pbuff);
    glBindBuffer(GL_ARRAY_BUFFER, Pbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*Pnt.size(),
                 &Pnt[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (Nrm.size() > 0) {
        GLuint Nbuff;
        glGenBuffers(1, &Nbuff);
        glBindBuffer(GL_ARRAY_BUFFER, Nbuff);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*Pnt.size(),
                     &Nrm[0][0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); }

    if (Tex.size() > 0) {
        GLuint Tbuff;
        glGenBuffers(1, &Tbuff);
        glBindBuffer(GL_ARRAY_BUFFER, Tbuff);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*Pnt.size(),
                     &Tex[0][0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); }

    if (Tan.size() > 0) {
        GLuint Dbuff;
        glGenBuffers(1, &Dbuff);
        glBindBuffer(GL_ARRAY_BUFFER, Dbuff);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*Pnt.size(),
                     &Tan[0][0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); }

    GLuint Ibuff;
    glGenBuffers(1, &Ibuff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibuff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*3*Tri.size(),
                 &Tri[0][0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    return vao;
}

void Model::ComputeSize()
{
    // Compute min/max
    minP = swizzle<X,Y,Z>(Pnt[0]);
    maxP = swizzle<X,Y,Z>(Pnt[0]);
    for (std::vector<vec4>::iterator p=Pnt.begin();  p<Pnt.end();  p++)
        for (int c=0;  c<3;  c++) {
            minP[c] = min(minP[c], (*p)[c]);
            maxP[c] = max(maxP[c], (*p)[c]); }

    center = (maxP+minP)/2.0f;
    size = 0.0;
    for (int c=0;  c<3;  c++)
        size = max(size, (maxP[c]-minP[c])/2.0f);

    float s = 1.0/size;
    modelTr = Scale(s,s,s)*Translate(-center[0], -center[1], -center[2]);
}

void Model::MakeVAO()
{
    if (Quad.size()) {
        vao = VaoFromQuads(Pnt, Nrm, Tex, Tan, Quad);
        count = Quad.size();
        shape = 4; }
    else {
        vao = VaoFromTris(Pnt, Nrm, Tex, Tan, Tri);
        count = Tri.size();
        shape = 3; }
}

void Model::DrawVAO()
{
    glBindVertexArray(vao);
    if (shape==4)
        glDrawElements(GL_QUADS, shape*count, GL_UNSIGNED_INT, 0);
    else
        glDrawElements(GL_TRIANGLES, shape*count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////////////
// Data for the Utah teapot.  It consists of a list of 306 control
// points, and 32 Bezier patches, each defined by 16 control points
// (specified as 1-based indices into the control point array).  This
// evaluates the Bernstein basis functions directly to compute the
// vertices.  This is not the most efficient, but it is the
// easiest. (Which is fine for an operation done once at startup
// time.)
unsigned int TeapotIndex[][16] = {
      1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
      4, 17, 18, 19,  8, 20, 21, 22, 12, 23, 24, 25, 16, 26, 27, 28,
     19, 29, 30, 31, 22, 32, 33, 34, 25, 35, 36, 37, 28, 38, 39, 40,
     31, 41, 42,  1, 34, 43, 44,  5, 37, 45, 46,  9, 40, 47, 48, 13,
     13, 14, 15, 16, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
     16, 26, 27, 28, 52, 61, 62, 63, 56, 64, 65, 66, 60, 67, 68, 69,
     28, 38, 39, 40, 63, 70, 71, 72, 66, 73, 74, 75, 69, 76, 77, 78,
     40, 47, 48, 13, 72, 79, 80, 49, 75, 81, 82, 53, 78, 83, 84, 57,
     57, 58, 59, 60, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
     60, 67, 68, 69, 88, 97, 98, 99, 92,100,101,102, 96,103,104,105,
     69, 76, 77, 78, 99,106,107,108,102,109,110,111,105,112,113,114,
     78, 83, 84, 57,108,115,116, 85,111,117,118, 89,114,119,120, 93,
    121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,
    124,137,138,121,128,139,140,125,132,141,142,129,136,143,144,133,
    133,134,135,136,145,146,147,148,149,150,151,152, 69,153,154,155,
    136,143,144,133,148,156,157,145,152,158,159,149,155,160,161, 69,
    162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,
    165,178,179,162,169,180,181,166,173,182,183,170,177,184,185,174,
    174,175,176,177,186,187,188,189,190,191,192,193,194,195,196,197,
    177,184,185,174,189,198,199,186,193,200,201,190,197,202,203,194,
    204,204,204,204,207,208,209,210,211,211,211,211,212,213,214,215,
    204,204,204,204,210,217,218,219,211,211,211,211,215,220,221,222,
    204,204,204,204,219,224,225,226,211,211,211,211,222,227,228,229,
    204,204,204,204,226,230,231,207,211,211,211,211,229,232,233,212,
    212,213,214,215,234,235,236,237,238,239,240,241,242,243,244,245,
    215,220,221,222,237,246,247,248,241,249,250,251,245,252,253,254,
    222,227,228,229,248,255,256,257,251,258,259,260,254,261,262,263,
    229,232,233,212,257,264,265,234,260,266,267,238,263,268,269,242,
    270,270,270,270,279,280,281,282,275,276,277,278,271,272,273,274,
    270,270,270,270,282,289,290,291,278,286,287,288,274,283,284,285,
    270,270,270,270,291,298,299,300,288,295,296,297,285,292,293,294,
    270,270,270,270,300,305,306,279,297,303,304,275,294,301,302,271};

vec3 TeapotPoints[] = {
    vec3(1.4,0.0,2.4), vec3(1.4,-0.784,2.4), vec3(0.784,-1.4,2.4),
    vec3(0.0,-1.4,2.4), vec3(1.3375,0.0,2.53125),
    vec3(1.3375,-0.749,2.53125), vec3(0.749,-1.3375,2.53125),
    vec3(0.0,-1.3375,2.53125), vec3(1.4375,0.0,2.53125),
    vec3(1.4375,-0.805,2.53125), vec3(0.805,-1.4375,2.53125),
    vec3(0.0,-1.4375,2.53125), vec3(1.5,0.0,2.4), vec3(1.5,-0.84,2.4),
    vec3(0.84,-1.5,2.4), vec3(0.0,-1.5,2.4), vec3(-0.784,-1.4,2.4),
    vec3(-1.4,-0.784,2.4), vec3(-1.4,0.0,2.4),
    vec3(-0.749,-1.3375,2.53125), vec3(-1.3375,-0.749,2.53125),
    vec3(-1.3375,0.0,2.53125), vec3(-0.805,-1.4375,2.53125),
    vec3(-1.4375,-0.805,2.53125), vec3(-1.4375,0.0,2.53125),
    vec3(-0.84,-1.5,2.4), vec3(-1.5,-0.84,2.4), vec3(-1.5,0.0,2.4),
    vec3(-1.4,0.784,2.4), vec3(-0.784,1.4,2.4), vec3(0.0,1.4,2.4),
    vec3(-1.3375,0.749,2.53125), vec3(-0.749,1.3375,2.53125),
    vec3(0.0,1.3375,2.53125), vec3(-1.4375,0.805,2.53125),
    vec3(-0.805,1.4375,2.53125), vec3(0.0,1.4375,2.53125),
    vec3(-1.5,0.84,2.4), vec3(-0.84,1.5,2.4), vec3(0.0,1.5,2.4),
    vec3(0.784,1.4,2.4), vec3(1.4,0.784,2.4), vec3(0.749,1.3375,2.53125),
    vec3(1.3375,0.749,2.53125), vec3(0.805,1.4375,2.53125),
    vec3(1.4375,0.805,2.53125), vec3(0.84,1.5,2.4), vec3(1.5,0.84,2.4),
    vec3(1.75,0.0,1.875), vec3(1.75,-0.98,1.875), vec3(0.98,-1.75,1.875),
    vec3(0.0,-1.75,1.875), vec3(2.0,0.0,1.35), vec3(2.0,-1.12,1.35),
    vec3(1.12,-2.0,1.35), vec3(0.0,-2.0,1.35), vec3(2.0,0.0,0.9),
    vec3(2.0,-1.12,0.9), vec3(1.12,-2.0,0.9), vec3(0.0,-2.0,0.9),
    vec3(-0.98,-1.75,1.875), vec3(-1.75,-0.98,1.875),
    vec3(-1.75,0.0,1.875), vec3(-1.12,-2.0,1.35), vec3(-2.0,-1.12,1.35),
    vec3(-2.0,0.0,1.35), vec3(-1.12,-2.0,0.9), vec3(-2.0,-1.12,0.9),
    vec3(-2.0,0.0,0.9), vec3(-1.75,0.98,1.875), vec3(-0.98,1.75,1.875),
    vec3(0.0,1.75,1.875), vec3(-2.0,1.12,1.35), vec3(-1.12,2.0,1.35),
    vec3(0.0,2.0,1.35), vec3(-2.0,1.12,0.9), vec3(-1.12,2.0,0.9),
    vec3(0.0,2.0,0.9), vec3(0.98,1.75,1.875), vec3(1.75,0.98,1.875),
    vec3(1.12,2.0,1.35), vec3(2.0,1.12,1.35), vec3(1.12,2.0,0.9),
    vec3(2.0,1.12,0.9), vec3(2.0,0.0,0.45), vec3(2.0,-1.12,0.45),
    vec3(1.12,-2.0,0.45), vec3(0.0,-2.0,0.45), vec3(1.5,0.0,0.225),
    vec3(1.5,-0.84,0.225), vec3(0.84,-1.5,0.225), vec3(0.0,-1.5,0.225),
    vec3(1.5,0.0,0.15), vec3(1.5,-0.84,0.15), vec3(0.84,-1.5,0.15),
    vec3(0.0,-1.5,0.15), vec3(-1.12,-2.0,0.45), vec3(-2.0,-1.12,0.45),
    vec3(-2.0,0.0,0.45), vec3(-0.84,-1.5,0.225), vec3(-1.5,-0.84,0.225),
    vec3(-1.5,0.0,0.225), vec3(-0.84,-1.5,0.15), vec3(-1.5,-0.84,0.15),
    vec3(-1.5,0.0,0.15), vec3(-2.0,1.12,0.45), vec3(-1.12,2.0,0.45),
    vec3(0.0,2.0,0.45), vec3(-1.5,0.84,0.225), vec3(-0.84,1.5,0.225),
    vec3(0.0,1.5,0.225), vec3(-1.5,0.84,0.15), vec3(-0.84,1.5,0.15),
    vec3(0.0,1.5,0.15), vec3(1.12,2.0,0.45), vec3(2.0,1.12,0.45),
    vec3(0.84,1.5,0.225), vec3(1.5,0.84,0.225), vec3(0.84,1.5,0.15),
    vec3(1.5,0.84,0.15), vec3(-1.6,0.0,2.025), vec3(-1.6,-0.3,2.025),
    vec3(-1.5,-0.3,2.25), vec3(-1.5,0.0,2.25), vec3(-2.3,0.0,2.025),
    vec3(-2.3,-0.3,2.025), vec3(-2.5,-0.3,2.25), vec3(-2.5,0.0,2.25),
    vec3(-2.7,0.0,2.025), vec3(-2.7,-0.3,2.025), vec3(-3.0,-0.3,2.25),
    vec3(-3.0,0.0,2.25), vec3(-2.7,0.0,1.8), vec3(-2.7,-0.3,1.8),
    vec3(-3.0,-0.3,1.8), vec3(-3.0,0.0,1.8), vec3(-1.5,0.3,2.25),
    vec3(-1.6,0.3,2.025), vec3(-2.5,0.3,2.25), vec3(-2.3,0.3,2.025),
    vec3(-3.0,0.3,2.25), vec3(-2.7,0.3,2.025), vec3(-3.0,0.3,1.8),
    vec3(-2.7,0.3,1.8), vec3(-2.7,0.0,1.575), vec3(-2.7,-0.3,1.575),
    vec3(-3.0,-0.3,1.35), vec3(-3.0,0.0,1.35), vec3(-2.5,0.0,1.125),
    vec3(-2.5,-0.3,1.125), vec3(-2.65,-0.3,0.9375),
    vec3(-2.65,0.0,0.9375), vec3(-2.0,-0.3,0.9), vec3(-1.9,-0.3,0.6),
    vec3(-1.9,0.0,0.6), vec3(-3.0,0.3,1.35), vec3(-2.7,0.3,1.575),
    vec3(-2.65,0.3,0.9375), vec3(-2.5,0.3,1.125), vec3(-1.9,0.3,0.6),
    vec3(-2.0,0.3,0.9), vec3(1.7,0.0,1.425), vec3(1.7,-0.66,1.425),
    vec3(1.7,-0.66,0.6), vec3(1.7,0.0,0.6), vec3(2.6,0.0,1.425),
    vec3(2.6,-0.66,1.425), vec3(3.1,-0.66,0.825), vec3(3.1,0.0,0.825),
    vec3(2.3,0.0,2.1), vec3(2.3,-0.25,2.1), vec3(2.4,-0.25,2.025),
    vec3(2.4,0.0,2.025), vec3(2.7,0.0,2.4), vec3(2.7,-0.25,2.4),
    vec3(3.3,-0.25,2.4), vec3(3.3,0.0,2.4), vec3(1.7,0.66,0.6),
    vec3(1.7,0.66,1.425), vec3(3.1,0.66,0.825), vec3(2.6,0.66,1.425),
    vec3(2.4,0.25,2.025), vec3(2.3,0.25,2.1), vec3(3.3,0.25,2.4),
    vec3(2.7,0.25,2.4), vec3(2.8,0.0,2.475), vec3(2.8,-0.25,2.475),
    vec3(3.525,-0.25,2.49375), vec3(3.525,0.0,2.49375),
    vec3(2.9,0.0,2.475), vec3(2.9,-0.15,2.475), vec3(3.45,-0.15,2.5125),
    vec3(3.45,0.0,2.5125), vec3(2.8,0.0,2.4), vec3(2.8,-0.15,2.4),
    vec3(3.2,-0.15,2.4), vec3(3.2,0.0,2.4), vec3(3.525,0.25,2.49375),
    vec3(2.8,0.25,2.475), vec3(3.45,0.15,2.5125), vec3(2.9,0.15,2.475),
    vec3(3.2,0.15,2.4), vec3(2.8,0.15,2.4), vec3(0.0,0.0,3.15),
    vec3(0.0,-0.002,3.15), vec3(0.002,0.0,3.15), vec3(0.8,0.0,3.15),
    vec3(0.8,-0.45,3.15), vec3(0.45,-0.8,3.15), vec3(0.0,-0.8,3.15),
    vec3(0.0,0.0,2.85), vec3(0.2,0.0,2.7), vec3(0.2,-0.112,2.7),
    vec3(0.112,-0.2,2.7), vec3(0.0,-0.2,2.7), vec3(-0.002,0.0,3.15),
    vec3(-0.45,-0.8,3.15), vec3(-0.8,-0.45,3.15), vec3(-0.8,0.0,3.15),
    vec3(-0.112,-0.2,2.7), vec3(-0.2,-0.112,2.7), vec3(-0.2,0.0,2.7),
    vec3(0.0,0.002,3.15), vec3(-0.8,0.45,3.15), vec3(-0.45,0.8,3.15),
    vec3(0.0,0.8,3.15), vec3(-0.2,0.112,2.7), vec3(-0.112,0.2,2.7),
    vec3(0.0,0.2,2.7), vec3(0.45,0.8,3.15), vec3(0.8,0.45,3.15),
    vec3(0.112,0.2,2.7), vec3(0.2,0.112,2.7), vec3(0.4,0.0,2.55),
    vec3(0.4,-0.224,2.55), vec3(0.224,-0.4,2.55), vec3(0.0,-0.4,2.55),
    vec3(1.3,0.0,2.55), vec3(1.3,-0.728,2.55), vec3(0.728,-1.3,2.55),
    vec3(0.0,-1.3,2.55), vec3(1.3,0.0,2.4), vec3(1.3,-0.728,2.4),
    vec3(0.728,-1.3,2.4), vec3(0.0,-1.3,2.4), vec3(-0.224,-0.4,2.55),
    vec3(-0.4,-0.224,2.55), vec3(-0.4,0.0,2.55), vec3(-0.728,-1.3,2.55),
    vec3(-1.3,-0.728,2.55), vec3(-1.3,0.0,2.55), vec3(-0.728,-1.3,2.4),
    vec3(-1.3,-0.728,2.4), vec3(-1.3,0.0,2.4), vec3(-0.4,0.224,2.55),
    vec3(-0.224,0.4,2.55), vec3(0.0,0.4,2.55), vec3(-1.3,0.728,2.55),
    vec3(-0.728,1.3,2.55), vec3(0.0,1.3,2.55), vec3(-1.3,0.728,2.4),
    vec3(-0.728,1.3,2.4), vec3(0.0,1.3,2.4), vec3(0.224,0.4,2.55),
    vec3(0.4,0.224,2.55), vec3(0.728,1.3,2.55), vec3(1.3,0.728,2.55),
    vec3(0.728,1.3,2.4), vec3(1.3,0.728,2.4), vec3(0.0,0.0,0.0),
    vec3(1.5,0.0,0.15), vec3(1.5,0.84,0.15), vec3(0.84,1.5,0.15),
    vec3(0.0,1.5,0.15), vec3(1.5,0.0,0.075), vec3(1.5,0.84,0.075),
    vec3(0.84,1.5,0.075), vec3(0.0,1.5,0.075), vec3(1.425,0.0,0.0),
    vec3(1.425,0.798,0.0), vec3(0.798,1.425,0.0), vec3(0.0,1.425,0.0),
    vec3(-0.84,1.5,0.15), vec3(-1.5,0.84,0.15), vec3(-1.5,0.0,0.15),
    vec3(-0.84,1.5,0.075), vec3(-1.5,0.84,0.075), vec3(-1.5,0.0,0.075),
    vec3(-0.798,1.425,0.0), vec3(-1.425,0.798,0.0), vec3(-1.425,0.0,0.0),
    vec3(-1.5,-0.84,0.15), vec3(-0.84,-1.5,0.15), vec3(0.0,-1.5,0.15),
    vec3(-1.5,-0.84,0.075), vec3(-0.84,-1.5,0.075), vec3(0.0,-1.5,0.075),
    vec3(-1.425,-0.798,0.0), vec3(-0.798,-1.425,0.0),
    vec3(0.0,-1.425,0.0), vec3(0.84,-1.5,0.15), vec3(1.5,-0.84,0.15),
    vec3(0.84,-1.5,0.075), vec3(1.5,-0.84,0.075), vec3(0.798,-1.425,0.0),
    vec3(1.425,-0.798,0.0)};

////////////////////////////////////////////////////////////////////////////////
// Builds a Vertex Array Object for the Utah teapot.  Each of the 32
// patches is represented by an n by n grid of quads.
Teapot::Teapot(const int n)
{
    //diffuseColor = vec3(0.5, 0.5, 0.1);
    //specularColor = vec3(1.0, 1.0, 1.0);
   // shininess = 120.0;



	diffuseColor = vec3(0.8, 0.8, 0.1);
	specularColor = vec3(0.3, 0.3, 0.3);
	shininess = 0.8;
    animate = true;

    int npatches = sizeof(TeapotIndex)/sizeof(TeapotIndex[0]); // Should be 32 patches for the teapot
    const int nv = npatches*(n+1)*(n+1);
    int nq = npatches*n*n;

    for (int p=0;  p<npatches;  p++)    { // For each patch
        for (int i=0;  i<=n; i++) {       // Grid in u direction
            float u = float(i)/n;
            for (int j=0;  j<=n; j++) { // Grid if v direction
                float v = float(j)/n;

                // Four u weights
                float u0 = (1.0-u)*(1.0-u)*(1.0-u);
                float u1 = 3.0*(1.0-u)*(1.0-u)*u;
                float u2 = 3.0*(1.0-u)*u*u;
                float u3 = u*u*u;

                // Three du weights
                float du0 = (1.0-u)*(1.0-u);
                float du1 = 2.0*(1.0-u)*u;
                float du2 = u*u;

                // Four v weights
                float v0 = (1.0-v)*(1.0-v)*(1.0-v);
                float v1 = 3.0*(1.0-v)*(1.0-v)*v;
                float v2 = 3.0*(1.0-v)*v*v;
                float v3 = v*v*v;

                // Three dv weights
                float dv0 = (1.0-v)*(1.0-v);
                float dv1 = 2.0*(1.0-v)*v;
                float dv2 = v*v;

                // Grab the 16 control points for Bezier patch.
                vec3* p00 = &TeapotPoints[TeapotIndex[p][ 0]-1];
                vec3* p01 = &TeapotPoints[TeapotIndex[p][ 1]-1];
                vec3* p02 = &TeapotPoints[TeapotIndex[p][ 2]-1];
                vec3* p03 = &TeapotPoints[TeapotIndex[p][ 3]-1];
                vec3* p10 = &TeapotPoints[TeapotIndex[p][ 4]-1];
                vec3* p11 = &TeapotPoints[TeapotIndex[p][ 5]-1];
                vec3* p12 = &TeapotPoints[TeapotIndex[p][ 6]-1];
                vec3* p13 = &TeapotPoints[TeapotIndex[p][ 7]-1];
                vec3* p20 = &TeapotPoints[TeapotIndex[p][ 8]-1];
                vec3* p21 = &TeapotPoints[TeapotIndex[p][ 9]-1];
                vec3* p22 = &TeapotPoints[TeapotIndex[p][10]-1];
                vec3* p23 = &TeapotPoints[TeapotIndex[p][11]-1];
                vec3* p30 = &TeapotPoints[TeapotIndex[p][12]-1];
                vec3* p31 = &TeapotPoints[TeapotIndex[p][13]-1];
                vec3* p32 = &TeapotPoints[TeapotIndex[p][14]-1];
                vec3* p33 = &TeapotPoints[TeapotIndex[p][15]-1];

                // Evaluate the Bezier patch at (u,v)
                vec3 V =
                    u0*v0*(*p00) + u0*v1*(*p01) + u0*v2*(*p02) + u0*v3*(*p03) +
                    u1*v0*(*p10) + u1*v1*(*p11) + u1*v2*(*p12) + u1*v3*(*p13) +
                    u2*v0*(*p20) + u2*v1*(*p21) + u2*v2*(*p22) + u2*v3*(*p23) +
                    u3*v0*(*p30) + u3*v1*(*p31) + u3*v2*(*p32) + u3*v3*(*p33);
                //*pp++ = vec4(V[0], V[1], V[2], 1.0);
                Pnt.push_back(vec4(V[0], V[1], V[2], 1.0));
                Tex.push_back(vec2(u,v));

                // Evaluate the u-tangent of the Bezier patch at (u,v)
                vec3 du =
                    du0*v0*(*p10-*p00) + du0*v1*(*p11-*p01) + du0*v2*(*p12-*p02) + du0*v3*(*p13-*p03) +
                    du1*v0*(*p20-*p10) + du1*v1*(*p21-*p11) + du1*v2*(*p22-*p12) + du1*v3*(*p23-*p13) +
                    du2*v0*(*p30-*p20) + du2*v1*(*p31-*p21) + du2*v2*(*p32-*p22) + du2*v3*(*p33-*p23);
                Tan.push_back(du);

                // Evaluate the v-tangent of the Bezier patch at (u,v)
                vec3 dv =
                    u0*dv0*(*p01-*p00) + u0*dv1*(*p02-*p01) + u0*dv2*(*p03-*p02) +
                    u1*dv0*(*p11-*p10) + u1*dv1*(*p12-*p11) + u1*dv2*(*p13-*p12) +
                    u2*dv0*(*p21-*p20) + u2*dv1*(*p22-*p21) + u2*dv2*(*p23-*p22) +
                    u3*dv0*(*p31-*p30) + u3*dv1*(*p32-*p31) + u3*dv2*(*p33-*p32);

                // Calculate the surface normal as the cross product of the two tangents.
                Nrm.push_back(cross(dv,du));

                //-(du[1]*dv[2]-du[2]*dv[1]);
                //*np++ = -(du[2]*dv[0]-du[0]*dv[2]);
                //*np++ = -(du[0]*dv[1]-du[1]*dv[0]);

                // Create a quad for all but the first edge vertices
                if (i>0 && j>0)
                    Quad.push_back(ivec4(p*(n+1)*(n+1) + (i-1)*(n+1) + (j-1),
                                          p*(n+1)*(n+1) + (i-1)*(n+1) + (j),
                                          p*(n+1)*(n+1) + (i  )*(n+1) + (j),
                                          p*(n+1)*(n+1) + (i  )*(n+1) + (j-1))); } } }
    ComputeSize();
    MakeVAO();
}

////////////////////////////////////////////////////////////////////////
// Generates a sphere with normals, texture coords, and tangent vectors.
Sphere::Sphere(const int n)
{
   // diffuseColor = vec3(0.5, 0.5, 1.0);
   // specularColor = vec3(1.0, 1.0, 1.0);
   // shininess = 120.0;

	diffuseColor = vec3(0.5, 0.5, 1.0);
	specularColor = vec3(.3, .3, .3);
	shininess = 0.3;

    float d = 2.0f*PI/float(n*2);
    for (int i=0;  i<=n*2;  i++) {
        float s = i*2.0f*PI/float(n*2);
        for (int j=0;  j<=n;  j++) {
            float t = j*PI/float(n);
            float x = cos(s)*sin(t);
            float y = sin(s)*sin(t);
            float z = cos(t);
            Pnt.push_back(vec4(x,y,z,1.0f));
            Nrm.push_back(vec3(x,y,z));
            Tex.push_back(vec2(s/(2*PI), t/PI));
            Tan.push_back(vec3(-sin(s), cos(s), 0.0));
            if (i>0 && j>0) {
                Quad.push_back(ivec4((i-1)*(n+1) + (j-1),
                                      (i-1)*(n+1) + (j),
                                      (i  )*(n+1) + (j),
                                      (i  )*(n+1) + (j-1))); } } }
    ComputeSize();
    MakeVAO();
}

////////////////////////////////////////////////////////////////////////
// Generates a plane with normals, texture coords, and tangent vectors
// from an n by n grid of small quads.  A single quad might have been
// sufficient, but that works poorly with the reflection map.
Ply::Ply(const char* name, const bool reverse)
{
   // diffuseColor = vec3(0.8, 0.8, 0.5);
   // specularColor = vec3(1.0, 1.0, 1.0);
   // shininess = 120.0;


	diffuseColor = vec3(0.8, 0.8, 0.5);
	specularColor = vec3(0.3, 0.3, 0.3);
	shininess = 0.8;

    // Open PLY file and read header;  Exit on any failure.
    p_ply ply = ply_open(name, NULL, 0, NULL);
    if (!ply) { throw std::exception(); }
    if (!ply_read_header(ply)) { throw std::exception(); }

    // Setup callback for verticescs
    ply_set_read_cb(ply, "vertex", "x", vertex_cb, this, 0);
    ply_set_read_cb(ply, "vertex", "y", vertex_cb, this, 1);
    ply_set_read_cb(ply, "vertex", "z", vertex_cb, this, 2);

    // Setup callback for faces
    ply_set_read_cb(ply, "face", "vertex_indices", face_cb, this, 0);

    // Read the PLY file filling the arrays via the callbacks.
    if (!ply_read(ply)) {printf("Failure in ply_read\n"); exit(-1); }


    // Zero out the vertex normals
    for (int i=0;  i<Pnt.size();  i++) {
        Tex.push_back(vec2(Pnt[i][0], Pnt[i][1]));
        Tan.push_back(vec3(1,0,0));
        Nrm.push_back(vec3(0,0,0)); }

    // Compute vertex normals: (For each face, compute a face
    // normal fn, and sum it into each of the face's vertex
    // normals.)
    for (int i=0;  i<Tri.size();  i++) {
        int i0 = Tri[i][0];
        int i1 = Tri[i][1];
        int i2 = Tri[i][2];

        vec4* v0 = &Pnt[i0];
        vec4* v1 = &Pnt[i1];
        vec4* v2 = &Pnt[i2];

        vec3 FN = cross(vec3(*v1-*v0), vec3(*v2-*v0));
        if (reverse) FN = -FN;
        FN = normalize(FN);

        Nrm[i0] += FN;
        Nrm[i1] += FN;
        Nrm[i2] += FN; }

    // Normalize the vertex normals.
    for (int i=0;  i<Pnt.size();  i++)
        Nrm[i] = normalize(Nrm[i]);

    ComputeSize();
    MakeVAO();
}


vec4 staticPnt;
vec3 staticNrm;
vec2 staticTex;
ivec3 staticTri;

// Vertex callback;  Must be static (stupid C++)
int Ply::vertex_cb(p_ply_argument argument) {
    long index;
    Ply *ply;
    ply_get_argument_user_data(argument, (void**)&ply, &index);
    double c = ply_get_argument_value(argument);
    staticPnt[index] = c;
    if (index==2) {
        staticPnt[3] = 1.0;
        ply->Pnt.push_back(staticPnt); }
    return 1;
}

// Face callback;  Must be static (stupid C++)
int Ply::face_cb(p_ply_argument argument) {
    long length, value_index;
    long index;
    Ply *ply;
    ply_get_argument_user_data(argument, (void**)&ply, &index);
    ply_get_argument_property(argument, NULL, &length, &value_index);

    if (value_index == -1) {
         }
    else {
        if (value_index<2) {
            staticTri[value_index] = (int)ply_get_argument_value(argument); }
        else if (value_index==2) {
            staticTri[2] = (int)ply_get_argument_value(argument);
            ply->Tri.push_back(staticTri); }
        else if (value_index==3) {
            staticTri[1] = staticTri[2];
            staticTri[2] = (int)ply_get_argument_value(argument);
            ply->Tri.push_back(staticTri); }}

    return 1;
}

////////////////////////////////////////////////////////////////////////
// Generates a plane with normals, texture coords, and tangent vectors
// from an n by n grid of small quads.  A single quad might have been
// sufficient, but that works poorly with the reflection map.
Ground::Ground(const float r, const int n)
{
    std::vector<vec4> Pnt;
    std::vector<vec3> Nrm;
    std::vector<vec2> Tex;
    std::vector<vec3> Tan;
    std::vector<ivec4> Quad;

    //diffuseColor = vec3(0.3, 0.2, 0.1);
    //specularColor = vec3(1.0, 1.0, 1.0);
    //shininess = 120.0;


	diffuseColor = vec3(0.3, 0.2, 0.1);
	specularColor = vec3(.03, .03, .03);
	shininess = 0.1;

    for (int i=0;  i<=n;  i++) {
        float s = i/float(n);
        for (int j=0;  j<=n;  j++) {
            float t = j/float(n);
            Pnt.push_back(vec4(s*2.0*r-r, t*2.0*r-r, -3.0, 1.0));
            Nrm.push_back(vec3(0.0, 0.0, 1.0));
            Tex.push_back(vec2(s, t));
            Tan.push_back(vec3(1.0, 0.0, 0.0));
            if (i>0 && j>0) {
                Quad.push_back(ivec4((i-1)*(n+1) + (j-1),
                                      (i-1)*(n+1) + (j),
                                      (i  )*(n+1) + (j),
                                      (i  )*(n+1) + (j-1))); } } }

    vao = VaoFromQuads(Pnt, Nrm, Tex, Tan, Quad);
    count = Quad.size();
    shape = 4;
}
