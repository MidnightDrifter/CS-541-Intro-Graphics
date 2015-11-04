#include <fstream>

#include <math.h>
#include "transform.h"
#define M_PI       3.14159265358979323846   // pi
// This is used to communicate a MAT4's address to OpenGL
float* MAT4::Pntr()
{
	return &(M[0][0]);
}

// Return a rotation matrix around an axis (0:X, 1:Y, 2:Z)
// by an angle measured in degrees.
// NOTE:  Make sure to convert degrees to radians before using sin and cos:
//        radians = degrees*PI/180

MAT4 Identity()
{

	MAT4 a = MAT4();
	return a;

}


MAT4 Rotate(const int i, const float theta)
{
	MAT4 R = Identity();

	float angle = (theta * M_PI) / 180;
	float cosine = cos(angle);
	float sine = sin(angle);
	//x
	if (i == 0)
	{

		R[1][1] = cosine;
		R[1][2] = -1 * sine;
		R[2][1] = sine;
		R[2][2] = cosine;

	}

	//y
	else if (i == 1)
	{
		R[0][0] = cosine;
		R[0][2] = sine;
		R[2][0] = -1 * sine;
		R[2][2] = cosine;

	}
	//z
	else
	{

		R[0][0] = cosine;
		R[0][1] = -1 * sine;
		R[1][0] = sine;
		R[1][1] = cosine;

	}


	return R;
}

// Return a scale matrix
MAT4 Scale(const vec3 s)
{
	//return Scale(s.x, s.y, s.z);
	 return Scale(s[0], s[1], s[2]);
}

MAT4 Scale(const float x, const float y, const float z)
{
	MAT4 S = Identity();

	S[0][0] = x;
	S[1][1] = y;
	S[2][2] = z;




	return S;
}

// Return a translation matrix
MAT4 Translate(const vec3 t)
{
	//return Translate(t.x, t.y, t.z);
	 return Translate(t[0], t[1], t[2]);
}

MAT4 Translate(const float x, const float y, const float z)
{
	MAT4 T = Identity();


	T[0][3] = x;
	T[1][3] = y;
	T[2][3] = z;


	return T;
}

// Returns a perspective projection matrix
MAT4 Perspective(const float rx, const float ry,
	const float front, const float back)
{
	MAT4 P = Identity();
	P[0][0] = 1 / rx;
	P[1][1] = 1 / ry;
	P[2][2] = (back + front) / (front - back);
	P[2][3] = (-2 * front*back) / (back - front);
	P[3][2] = -1;
	P[3][3] = 0;




	return P;
}

// Multiplies two 4x4 matrices
MAT4 operator* (const MAT4 A, const MAT4 B)
{
	MAT4 M = Identity();
	int i, j, k;
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			if (i == j)
			{
				M[i][j] = 0;
			}


			for (k = 0; k<4; k++)
			{

				M[i][j] += A[i][k] * B[k][j];


			}

		}
	}


	return M;
}


MAT4 makeEyeViewingTransform(vec3 view, vec3 up, vec3 w)
{
	MAT4 m = Identity();

	vec3 viewN = normalize(view);
	vec3 upN = normalize(up);
	vec3 wN = normalize(w);


	m[0][0] = viewN[0];
	m[0][1] = viewN[1];
	m[0][2] = viewN[2];
	m[0][3] = 0;
	m[1][0] = upN[0];
	m[1][1] = upN[1];
	m[1][2] = upN[2];
	m[1][3] = 0;



	m[2][0] = wN[0];
	m[2][1] = wN[1];
	m[2][2] = wN[2];
	m[2][3] = 0;


	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

	return m;

}

////////////////////////////////////////////////////////////////////////
// Calculates the inverse of a matrix by performing the gaussian
// matrix reduction with partial pivoting followed by
// back/substitution with the loops manually unrolled.
//
// Taken from Mesa implementation of OpenGL:  http://mesa3d.sourceforge.net/
////////////////////////////////////////////////////////////////////////
#define MAT(m,r,c) ((m)[r][c])
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }

MAT4 MAT4::inverse()
{
	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;

	// for (int i=0;  i<4;  i++)
	//     for (int j=0;  j<4;  j++)
	//         printf("%9.4f%c", mat[i][j], j==3?'\n':' ');
	// printf("\n");

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = MAT(*this, 0, 0);
	r0[1] = MAT(*this, 0, 1);
	r0[2] = MAT(*this, 0, 2);
	r0[3] = MAT(*this, 0, 3);
	r0[4] = 1.0;
	r0[5] = r0[6] = r0[7] = 0.0;

	r1[0] = MAT(*this, 1, 0);
	r1[1] = MAT(*this, 1, 1);
	r1[2] = MAT(*this, 1, 2);
	r1[3] = MAT(*this, 1, 3);
	r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0;

	r2[0] = MAT(*this, 2, 0);
	r2[1] = MAT(*this, 2, 1);
	r2[2] = MAT(*this, 2, 2);
	r2[3] = MAT(*this, 2, 3);
	r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0;

	r3[0] = MAT(*this, 3, 0);
	r3[1] = MAT(*this, 3, 1);
	r3[2] = MAT(*this, 3, 2);
	r3[3] = MAT(*this, 3, 3);
	r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	/* choose pivot - or die */
	if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
	if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
	if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
	if (0.0 == r0[0])  throw "Matrix in not invertable";

	/* eliminate first variable     */
	m1 = r1[0] / r0[0]; m2 = r2[0] / r0[0]; m3 = r3[0] / r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5];
	if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6];
	if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7];
	if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
	if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
	if (0.0 == r1[1])  throw "Matrix in not invertable";

	/* eliminate second variable */
	m2 = r2[1] / r1[1]; m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
	if (0.0 == r2[2])  throw "Matrix in not invertable";

	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3];
	r3[4] -= m3 * r2[4];
	r3[5] -= m3 * r2[5];
	r3[6] -= m3 * r2[6];
	r3[7] -= m3 * r2[7];

	/* last check */
	if (0.0 == r3[3]) throw "Matrix in not invertable";

	s = 1.0F / r3[3];             /* now back substitute row 3 */
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

	m2 = r2[3];                 /* now back substitute row 2 */
	s = 1.0F / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2);
	r2[5] = s * (r2[5] - r3[5] * m2);
	r2[6] = s * (r2[6] - r3[6] * m2);
	r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1;
	r1[5] -= r3[5] * m1;
	r1[6] -= r3[6] * m1;
	r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0;
	r0[5] -= r3[5] * m0;
	r0[6] -= r3[6] * m0;
	r0[7] -= r3[7] * m0;

	m1 = r1[2];                 /* now back substitute row 1 */
	s = 1.0F / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1);
	r1[5] = s * (r1[5] - r2[5] * m1);
	r1[6] = s * (r1[6] - r2[6] * m1);
	r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0;
	r0[5] -= r2[5] * m0;
	r0[6] -= r2[6] * m0;
	r0[7] -= r2[7] * m0;

	m0 = r0[1];                 /* now back substitute row 0 */
	s = 1.0F / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0);
	r0[5] = s * (r0[5] - r1[5] * m0);
	r0[6] = s * (r0[6] - r1[6] * m0);
	r0[7] = s * (r0[7] - r1[7] * m0);

	MAT4 inv;
	MAT(inv, 0, 0) = r0[4];
	MAT(inv, 0, 1) = r0[5],
		MAT(inv, 0, 2) = r0[6];
	MAT(inv, 0, 3) = r0[7],
		MAT(inv, 1, 0) = r1[4];
	MAT(inv, 1, 1) = r1[5],
		MAT(inv, 1, 2) = r1[6];
	MAT(inv, 1, 3) = r1[7],
		MAT(inv, 2, 0) = r2[4];
	MAT(inv, 2, 1) = r2[5],
		MAT(inv, 2, 2) = r2[6];
	MAT(inv, 2, 3) = r2[7],
		MAT(inv, 3, 0) = r3[4];
	MAT(inv, 3, 1) = r3[5],
		MAT(inv, 3, 2) = r3[6];
	MAT(inv, 3, 3) = r3[7];
	return inv;
}
