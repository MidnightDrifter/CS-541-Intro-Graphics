
#ifndef _TRANSFORM_
#define _TRANSFORM_

#include <glm/glm.hpp>
using namespace glm;

typedef float ROW4[4];

class MAT4
{
public:
    float M[4][4];

    // Initilaize all matrices to the identity
    MAT4()
    {
        for( int i=0; i<4; i++ )
            for( int j=0; j<4; j++)
                M[i][j] = i==j ? 1.0 : 0.0;
    }

    // Some indexing operations for the matrix
    ROW4& operator[](const int i)  { return M[i]; }
    const ROW4& operator[](const int i) const { return M[i]; }

    // Used to communicate to OpenGL
    float* Pntr();

    // Calculate the inverse matrix.
    MAT4 inverse();
};

MAT4 Rotate(const int i, const float theta);
MAT4 Scale(const float x, const float y, const float z);
MAT4 Translate(const vec3 t);
MAT4 Translate(const float x, const float y, const float z);
MAT4 Perspective(const float rx, const float ry,
                 const float front, const float back);
MAT4 operator* (const MAT4 A, const MAT4 B);

#endif
