#include "Precompiled.h"
#include "Matrix4.h"

namespace Math
{

Matrix4& Matrix4::Transpose()
{
	std::swap(m[1], m[4]);
	std::swap(m[2], m[8]);
	std::swap(m[3], m[12]);
	std::swap(m[6], m[9]);
	std::swap(m[7], m[13]);
	std::swap(m[11], m[14]);

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// inverse 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::Invert()
{
	// If the 4th row is [0,0,0,1] then it is affine matrix and
	// it has no projective transformation.
	if (m[3] == 0.0f && m[7] == 0.0f && m[11] == 0.0f && m[15] == 1.0f)
		this->InvertAffine();
	else
	{
		this->InvertGeneral();
	}

	return *this;
}



///////////////////////////////////////////////////////////////////////////////
// compute the inverse of 4x4 Euclidean transformation matrix
//
// Euclidean transformation is translation, rotation, and reflection.
// With Euclidean transform, only the position and orientation of the object
// will be changed. Euclidean transform does not change the shape of an object
// (no scaling). Length and angle are reserved.
//
// Use inverseAffine() if the matrix has scale and shear transformation.
//
// M = [ R | T ]
//     [ --+-- ]    (R denotes 3x3 rotation/reflection matrix)
//     [ 0 | 1 ]    (T denotes 1x3 translation matrix)
//
// y = M*x  ->  y = R*x + T  ->  x = R^-1*(y - T)  ->  x = R^T*y - R^T*T
// (R is orthogonal,  R^-1 = R^T)
//
//  [ R | T ]-1    [ R^T | -R^T * T ]    (R denotes 3x3 rotation matrix)
//  [ --+-- ]   =  [ ----+--------- ]    (T denotes 1x3 translation)
//  [ 0 | 1 ]      [  0  |     1    ]    (R^T denotes R-transpose)
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::InvertEuclidean()
{
	// transpose 3x3 rotation matrix part
	// | R^T | 0 |
	// | ----+-- |
	// |  0  | 1 |
	float tmp;
	tmp = m[1];  m[1] = m[4];  m[4] = tmp;
	tmp = m[2];  m[2] = m[8];  m[8] = tmp;
	tmp = m[6];  m[6] = m[9];  m[9] = tmp;

	// compute translation part -R^T * T
	// | 0 | -R^T x |
	// | --+------- |
	// | 0 |   0    |
	float x = m[12];
	float y = m[13];
	float z = m[14];
	m[12] = -(m[0] * x + m[4] * y + m[8] * z);
	m[13] = -(m[1] * x + m[5] * y + m[9] * z);
	m[14] = -(m[2] * x + m[6] * y + m[10] * z);

	// last row should be unchanged (0,0,0,1)

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// compute the inverse of a 4x4 affine transformation matrix
//
// Affine transformations are generalizations of Euclidean transformations.
// Affine transformation includes translation, rotation, reflection, scaling,
// and shearing. Length and angle are NOT preserved.
// M = [ R | T ]
//     [ --+-- ]    (R denotes 3x3 rotation/scale/shear matrix)
//     [ 0 | 1 ]    (T denotes 1x3 translation matrix)
//
// y = M*x  ->  y = R*x + T  ->  x = R^-1*(y - T)  ->  x = R^-1*y - R^-1*T
//
//  [ R | T ]-1   [ R^-1 | -R^-1 * T ]
//  [ --+-- ]   = [ -----+---------- ]
//  [ 0 | 1 ]     [  0   +     1     ]
///////////////////////////////////////////////////////////////////////////////
Matrix4 Matrix4::InvertAffine()
{
	Matrix4 result;

	// compute upper left 3x3 matrix determinant
	float cofactor0 = m[5] * m[10] - m[6] * m[9];
	float cofactor4 = m[2] * m[9] - m[1] * m[10];
	float cofactor8 = m[1] * m[6] - m[2] * m[5];
	float det = m[0] * cofactor0 + m[4] * cofactor4 + m[8] * cofactor8;
	if (fabsf(det) < EPSILON)
	{
		return result;
	}

	// create adjunct matrix and multiply by 1/det to get upper 3x3
	float invDet = 1.0f / det;
	result.m[0] = invDet*cofactor0;
	result.m[1] = invDet*cofactor4;
	result.m[2] = invDet*cofactor8;

	result.m[4] = invDet*(m[6] * m[8] - m[4] * m[10]);
	result.m[5] = invDet*(m[0] * m[10] - m[2] * m[8]);
	result.m[6] = invDet*(m[2] * m[4] - m[0] * m[6]);

	result.m[8] = invDet*(m[4] * m[9] - m[5] * m[8]);
	result.m[9] = invDet*(m[1] * m[8] - m[0] * m[9]);
	result.m[10] = invDet*(m[0] * m[5] - m[1] * m[4]);

	// multiply -translation by inverted 3x3 to get its inverse

	result.m[12] = -result.m[0] * m[12] - result.m[4] * m[13] - result.m[8] * m[14];
	result.m[13] = -result.m[1] * m[12] - result.m[5] * m[13] - result.m[9] * m[14];
	result.m[14] = -result.m[2] * m[12] - result.m[6] * m[13] - result.m[10] * m[14];

	return result;

}   // End of AffineInverse()



	///////////////////////////////////////////////////////////////////////////////
	// inverse matrix using matrix partitioning (blockwise inverse)
	// It devides a 4x4 matrix into 4 of 2x2 matrices. It works in case of where
	// det(A) != 0. If not, use the generic inverse method
	// inverse formula.
	// M = [ A | B ]    A, B, C, D are 2x2 matrix blocks
	//     [ --+-- ]    det(M) = |A| * |D - ((C * A^-1) * B)|
	//     [ C | D ]
	//
	// M^-1 = [ A' | B' ]   A' = A^-1 - (A^-1 * B) * C'
	//        [ ---+--- ]   B' = (A^-1 * B) * -D'
	//        [ C' | D' ]   C' = -D' * (C * A^-1)
	//                      D' = (D - ((C * A^-1) * B))^-1
	//
	// NOTE: I wrap with () if it it used more than once.
	//       The matrix is invertable even if det(A)=0, so must check det(A) before
	//       calling this function, and use invertGeneric() instead.
	///////////////////////////////////////////////////////////////////////////////
	//Matrix4& Matrix4::InvertProjective()
	//{
	//	// partition
	//	Matrix2 a(m[0], m[1], m[4], m[5]);
	//	Matrix2 b(m[8], m[9], m[12], m[13]);
	//	Matrix2 c(m[2], m[3], m[6], m[7]);
	//	Matrix2 d(m[10], m[11], m[14], m[15]);
	//
	//	// pre-compute repeated parts
	//	a.invert();             // A^-1
	//	Matrix2 ab = a * b;     // A^-1 * B
	//	Matrix2 ca = c * a;     // C * A^-1
	//	Matrix2 cab = ca * b;   // C * A^-1 * B
	//	Matrix2 dcab = d - cab; // D - C * A^-1 * B
	//
	//							// check determinant if |D - C * A^-1 * B| = 0
	//							//NOTE: this function assumes det(A) is already checked. if |A|=0 then,
	//							//      cannot use this function.
	//	float determinant = dcab[0] * dcab[3] - dcab[1] * dcab[2];
	//	if (fabs(determinant) <= EPSILON)
	//	{
	//		return identity();
	//	}
	//
	//	// compute D' and -D'
	//	Matrix2 d1 = dcab;      //  (D - C * A^-1 * B)
	//	d1.invert();            //  (D - C * A^-1 * B)^-1
	//	Matrix2 d2 = -d1;       // -(D - C * A^-1 * B)^-1
	//
	//							// compute C'
	//	Matrix2 c1 = d2 * ca;   // -D' * (C * A^-1)
	//
	//							// compute B'
	//	Matrix2 b1 = ab * d2;   // (A^-1 * B) * -D'
	//
	//							// compute A'
	//	Matrix2 a1 = a - (ab * c1); // A^-1 - (A^-1 * B) * C'
	//
	//								// assemble inverse matrix
	//	m[0] = a1[0];  m[4] = a1[2]; /*|*/ m[8] = b1[0];  m[12] = b1[2];
	//	m[1] = a1[1];  m[5] = a1[3]; /*|*/ m[9] = b1[1];  m[13] = b1[3];
	//	/*-----------------------------+-----------------------------*/
	//	m[2] = c1[0];  m[6] = c1[2]; /*|*/ m[10] = d1[0];  m[14] = d1[2];
	//	m[3] = c1[1];  m[7] = c1[3]; /*|*/ m[11] = d1[1];  m[15] = d1[3];
	//
	//	return *this;
	//}



	///////////////////////////////////////////////////////////////////////////////
	// compute the inverse of a general 4x4 matrix using Cramer's Rule
	// If cannot find inverse, return indentity matrix
	// M^-1 = adj(M) / det(M)
	///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::InvertGeneral()
{
	// get cofactors of minor matrices
	float cofactor0 = GetCofactor(m[5], m[6], m[7], m[9], m[10], m[11], m[13], m[14], m[15]);
	float cofactor1 = GetCofactor(m[4], m[6], m[7], m[8], m[10], m[11], m[12], m[14], m[15]);
	float cofactor2 = GetCofactor(m[4], m[5], m[7], m[8], m[9], m[11], m[12], m[13], m[15]);
	float cofactor3 = GetCofactor(m[4], m[5], m[6], m[8], m[9], m[10], m[12], m[13], m[14]);

	// get determinant
	float determinant = m[0] * cofactor0 - m[1] * cofactor1 + m[2] * cofactor2 - m[3] * cofactor3;
	if (fabs(determinant) <= EPSILON)
	{
		return Identity();
	}

	// get rest of cofactors for adj(M)
	float cofactor4 = GetCofactor(m[1], m[2], m[3], m[9], m[10], m[11], m[13], m[14], m[15]);
	float cofactor5 = GetCofactor(m[0], m[2], m[3], m[8], m[10], m[11], m[12], m[14], m[15]);
	float cofactor6 = GetCofactor(m[0], m[1], m[3], m[8], m[9], m[11], m[12], m[13], m[15]);
	float cofactor7 = GetCofactor(m[0], m[1], m[2], m[8], m[9], m[10], m[12], m[13], m[14]);

	float cofactor8 = GetCofactor(m[1], m[2], m[3], m[5], m[6], m[7], m[13], m[14], m[15]);
	float cofactor9 = GetCofactor(m[0], m[2], m[3], m[4], m[6], m[7], m[12], m[14], m[15]);
	float cofactor10 = GetCofactor(m[0], m[1], m[3], m[4], m[5], m[7], m[12], m[13], m[15]);
	float cofactor11 = GetCofactor(m[0], m[1], m[2], m[4], m[5], m[6], m[12], m[13], m[14]);

	float cofactor12 = GetCofactor(m[1], m[2], m[3], m[5], m[6], m[7], m[9], m[10], m[11]);
	float cofactor13 = GetCofactor(m[0], m[2], m[3], m[4], m[6], m[7], m[8], m[10], m[11]);
	float cofactor14 = GetCofactor(m[0], m[1], m[3], m[4], m[5], m[7], m[8], m[9], m[11]);
	float cofactor15 = GetCofactor(m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9], m[10]);

	// build inverse matrix = adj(M) / det(M)
	// adjugate of M is the transpose of the cofactor matrix of M
	float invDeterminant = 1.0f / determinant;
	m[0] = invDeterminant * cofactor0;
	m[1] = -invDeterminant * cofactor4;
	m[2] = invDeterminant * cofactor8;
	m[3] = -invDeterminant * cofactor12;

	m[4] = -invDeterminant * cofactor1;
	m[5] = invDeterminant * cofactor5;
	m[6] = -invDeterminant * cofactor9;
	m[7] = invDeterminant * cofactor13;

	m[8] = invDeterminant * cofactor2;
	m[9] = -invDeterminant * cofactor6;
	m[10] = invDeterminant * cofactor10;
	m[11] = -invDeterminant * cofactor14;

	m[12] = -invDeterminant * cofactor3;
	m[13] = invDeterminant * cofactor7;
	m[14] = -invDeterminant * cofactor11;
	m[15] = invDeterminant * cofactor15;

	return *this;
}



///////////////////////////////////////////////////////////////////////////////
// return determinant of 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
float Matrix4::GetDeterminant()
{
	return m[0] * GetCofactor(m[5], m[6], m[7], m[9], m[10], m[11], m[13], m[14], m[15]) -
		m[1] * GetCofactor(m[4], m[6], m[7], m[8], m[10], m[11], m[12], m[14], m[15]) +
		m[2] * GetCofactor(m[4], m[5], m[7], m[8], m[9], m[11], m[12], m[13], m[15]) -
		m[3] * GetCofactor(m[4], m[5], m[6], m[8], m[9], m[10], m[12], m[13], m[14]);
}



///////////////////////////////////////////////////////////////////////////////
// compute cofactor of 3x3 minor matrix without sign
// input params are 9 elements of the minor matrix
// NOTE: The caller must know its sign.
///////////////////////////////////////////////////////////////////////////////
float Matrix4::GetCofactor(	float m0, float m1, float m2,
							float m3, float m4, float m5,
							float m6, float m7, float m8)
{
	return	m0 * (m4 * m8 - m5 * m7)-
			m1 * (m3 * m8 - m5 * m6)+
			m2 * (m3 * m7 - m4 * m6);
}



///////////////////////////////////////////////////////////////////////////////
// translate this matrix by (x, y, z)
///////////////////////////////////////////////////////////////////////////////
//Matrix4& Matrix4::Translate(const Vector3& v)
//{
//	return Translate(v.x, v.y, v.z);
//}

Matrix4& Matrix4::Translate(float x, float y, float z)
{
	m[0] += m[3] * x;   m[4] += m[7] * x;   m[8] += m[11] * x;   m[12] += m[15] * x;
	m[1] += m[3] * y;   m[5] += m[7] * y;   m[9] += m[11] * y;   m[13] += m[15] * y;
	m[2] += m[3] * z;   m[6] += m[7] * z;   m[10] += m[11] * z;   m[14] += m[15] * z;

	return *this;
}



///////////////////////////////////////////////////////////////////////////////
// uniform scale
///////////////////////////////////////////////////////////////////////////////
Matrix4& Matrix4::Scale(float s)
{
	return Scale(s, s, s);
}

Matrix4& Matrix4::Scale(float x, float y, float z)
{
	m[0] *= x;   m[4] *= x;   m[8] *= x;   m[12] *= x;
	m[1] *= y;   m[5] *= y;   m[9] *= y;   m[13] *= y;
	m[2] *= z;   m[6] *= z;   m[10] *= z;   m[14] *= z;
	return *this;
}



///////////////////////////////////////////////////////////////////////////////
// build a rotation matrix with given angle(degree) and rotation axis, then
// multiply it with this object
///////////////////////////////////////////////////////////////////////////////
//Matrix4& Matrix4::Rotate(float angle, const Vector3& axis)
//{
//	return Rotate(angle, axis.x, axis.y, axis.z);
//}

Matrix4& Matrix4::Rotate(float angle, float x, float y, float z)
{
	float c = cosf(angle * DEG2RAD);    // cosine
	float s = sinf(angle * DEG2RAD);    // sine
	float c1 = 1.0f - c;                // 1 - c
	float m0 = m[0], m4 = m[4], m8 = m[8], m12 = m[12],
		m1 = m[1], m5 = m[5], m9 = m[9], m13 = m[13],
		m2 = m[2], m6 = m[6], m10 = m[10], m14 = m[14];

	// build rotation matrix
	float r0 = x * x * c1 + c;
	float r1 = x * y * c1 + z * s;
	float r2 = x * z * c1 - y * s;
	float r4 = x * y * c1 - z * s;
	float r5 = y * y * c1 + c;
	float r6 = y * z * c1 + x * s;
	float r8 = x * z * c1 + y * s;
	float r9 = y * z * c1 - x * s;
	float r10 = z * z * c1 + c;

	// multiply rotation matrix
	m[0] = r0 * m0 + r4 * m1 + r8 * m2;
	m[1] = r1 * m0 + r5 * m1 + r9 * m2;
	m[2] = r2 * m0 + r6 * m1 + r10* m2;
	m[4] = r0 * m4 + r4 * m5 + r8 * m6;
	m[5] = r1 * m4 + r5 * m5 + r9 * m6;
	m[6] = r2 * m4 + r6 * m5 + r10* m6;
	m[8] = r0 * m8 + r4 * m9 + r8 * m10;
	m[9] = r1 * m8 + r5 * m9 + r9 * m10;
	m[10] = r2 * m8 + r6 * m9 + r10* m10;
	m[12] = r0 * m12 + r4 * m13 + r8 * m14;
	m[13] = r1 * m12 + r5 * m13 + r9 * m14;
	m[14] = r2 * m12 + r6 * m13 + r10* m14;

	return *this;
}

Matrix4& Matrix4::RotateX(float angle)
{
	float c = cosf(angle * DEG2RAD);
	float s = sinf(angle * DEG2RAD);
	float m1 = m[1], m2 = m[2],
		m5 = m[5], m6 = m[6],
		m9 = m[9], m10 = m[10],
		m13 = m[13], m14 = m[14];

	m[1] = m1 * c + m2 *-s;
	m[2] = m1 * s + m2 * c;
	m[5] = m5 * c + m6 *-s;
	m[6] = m5 * s + m6 * c;
	m[9] = m9 * c + m10*-s;
	m[10] = m9 * s + m10* c;
	m[13] = m13* c + m14*-s;
	m[14] = m13* s + m14* c;

	return *this;
}

Matrix4& Matrix4::RotateY(float angle)
{
	float c = cosf(angle * DEG2RAD);
	float s = sinf(angle * DEG2RAD);
	float	m0 = m[0],		m2 = m[2],
			m4 = m[4],		m6 = m[6],
			m8 = m[8],		m10 = m[10],
			m12 = m[12],	m14 = m[14];

	m[0] = m0 * c + m2 * s;
	m[2] = m0 *-s + m2 * c;
	m[4] = m4 * c + m6 * s;
	m[6] = m4 *-s + m6 * c;
	m[8] = m8 * c + m10* s;
	m[10] = m8 *-s + m10* c;
	m[12] = m12* c + m14* s;
	m[14] = m12*-s + m14* c;

	return *this;
}

Matrix4& Matrix4::RotateZ(float angle)
{
	float c = cosf(angle * DEG2RAD);
	float s = sinf(angle * DEG2RAD);
	float m0 = m[0], m1 = m[1],
		m4 = m[4], m5 = m[5],
		m8 = m[8], m9 = m[9],
		m12 = m[12], m13 = m[13];

	m[0] = m0 * c + m1 *-s;
	m[1] = m0 * s + m1 * c;
	m[4] = m4 * c + m5 *-s;
	m[5] = m4 * s + m5 * c;
	m[8] = m8 * c + m9 *-s;
	m[9] = m8 * s + m9 * c;
	m[12] = m12* c + m13*-s;
	m[13] = m12* s + m13* c;

	return *this;
}
}