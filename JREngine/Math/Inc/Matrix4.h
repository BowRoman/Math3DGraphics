#pragma once

const float DEG2RAD = 3.141593f / 180;
const float EPSILON = 0.00001f;

class Matrix4
{
public:
	// constructors
	Matrix4() { Identity(); }
	Matrix4(const float source[16]);
	Matrix4(float m00, float m01, float m02, float m03, // 1st row
			float m04, float m05, float m06, float m07, // 2nd row
			float m08, float m09, float m10, float m11, // 3rd row
			float m12, float m13, float m14, float m15);// 4th row

	void Set(const float source[16]);
	void Set(float m00, float m01, float m02, float m03, // 1st row
			 float m04, float m05, float m06, float m07, // 2nd row
			 float m08, float m09, float m10, float m11, // 3rd row
			 float m12, float m13, float m14, float m15);// 4th row

	void SetRow(int index, const float row[4]);
	// void SetRow(int index, const Vector4& v);
	// void SetRow(int index, const Vector3& v);
	void SetColumn(int index, const float col[4]);
	// void SetColumn(int index, const Vector4& v);
	// void SetColumn(int index, const Vector3& v);

	const float* Get() const;
	const float* GetTranspose();                        // return transposed matrix
	float        GetDeterminant();

	Matrix4& Identity();
	Matrix4& Transpose();                            // transpose itself and return reference
	Matrix4& Invert();                               // check best inverse method before inverse
	Matrix4& InvertEuclidean();                      // inverse of Euclidean transform matrix
	Matrix4& InvertAffine();                         // inverse of affine transform matrix
	Matrix4& InvertProjective();                     // inverse of projective matrix using partitioning
	Matrix4& InvertGeneral();                        // inverse of generic matrix

					  								 // transform matrix
	Matrix4& Translate(float x, float y, float z);   // translation by (x,y,z)
	// Matrix4& Translate(const Vector3& v);            //
	// Matrix4& Rotate(float angle, const Vector3& axis); // rotate angle(degree) along the given axix
	Matrix4& Rotate(float angle, float x, float y, float z);
	Matrix4& RotateX(float angle);                   // rotate on X-axis with degree
	Matrix4& RotateY(float angle);                   // rotate on Y-axis with degree
	Matrix4& RotateZ(float angle);                   // rotate on Z-axis with degree
	Matrix4& Scale(float scale);                     // uniform scale
	Matrix4& Scale(float sx, float sy, float sz);    // scale by (sx, sy, sz) on each axis

														// operators
	Matrix4     operator+(const Matrix4& rhs) const;    // add rhs
	Matrix4     operator-(const Matrix4& rhs) const;    // subtract rhs
	Matrix4&    operator+=(const Matrix4& rhs);         // add rhs and update this object
	Matrix4&    operator-=(const Matrix4& rhs);         // subtract rhs and update this object
	// Vector4     operator*(const Vector4& rhs) const;    // multiplication: v' = M * v
	// Vector3     operator*(const Vector3& rhs) const;    // multiplication: v' = M * v
	Matrix4     operator*(const Matrix4& rhs) const;    // multiplication: M3 = M1 * M2
	Matrix4&    operator*=(const Matrix4& rhs);         // multiplication: M1' = M1 * M2
	bool        operator==(const Matrix4& rhs) const;   // exact compare, no epsilon
	bool        operator!=(const Matrix4& rhs) const;   // exact compare, no epsilon
	float       operator[](int index) const;            // subscript operator v[0], v[1]
	float&      operator[](int index);                  // subscript operator v[0], v[1]

	friend Matrix4 operator-(const Matrix4& m);                     // unary operator (-)
	friend Matrix4 operator*(float scalar, const Matrix4& m);       // pre-multiplication
	// friend Vector3 operator*(const Vector3& vec, const Matrix4& m); // pre-multiplication
	// friend Vector4 operator*(const Vector4& vec, const Matrix4& m); // pre-multiplication

protected:

private:
	float GetCofactor(float m0, float m1, float m2,
					  float m3, float m4, float m5,
					  float m6, float m7, float m8);

	float m[16];
	float tm[16]; // transposed m

};

inline Matrix4::Matrix4(const float src[16])
{
	Set(src);
}

inline Matrix4::Matrix4(float m00, float m01, float m02, float m03,
	float m04, float m05, float m06, float m07,
	float m08, float m09, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	Set(m00, m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15);
}

inline void Matrix4::Set(const float src[16])
{
	m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
	m[4] = src[4];  m[5] = src[5];  m[6] = src[6];  m[7] = src[7];
	m[8] = src[8];  m[9] = src[9];  m[10] = src[10]; m[11] = src[11];
	m[12] = src[12]; m[13] = src[13]; m[14] = src[14]; m[15] = src[15];
}

inline void Matrix4::Set(float m00, float m01, float m02, float m03,
						 float m04, float m05, float m06, float m07,
						 float m08, float m09, float m10, float m11,
						 float m12, float m13, float m14, float m15)
{
	m[0] = m00;  m[1] = m01;  m[2] = m02;  m[3] = m03;
	m[4] = m04;  m[5] = m05;  m[6] = m06;  m[7] = m07;
	m[8] = m08;  m[9] = m09;  m[10] = m10;  m[11] = m11;
	m[12] = m12;  m[13] = m13;  m[14] = m14;  m[15] = m15;
}

inline void Matrix4::SetRow(int index, const float row[4])
{
	m[index] = row[0];  m[index + 4] = row[1];  m[index + 8] = row[2];  m[index + 12] = row[3];
}

//inline void Matrix4::SetRow(int index, const Vector4& v)
//{
//	m[index] = v.x;  m[index + 4] = v.y;  m[index + 8] = v.z;  m[index + 12] = v.w;
//}
//
//inline void Matrix4::SetRow(int index, const Vector3& v)
//{
//	m[index] = v.x;  m[index + 4] = v.y;  m[index + 8] = v.z;
//}

inline void Matrix4::SetColumn(int index, const float col[4])
{
	m[index * 4] = col[0];  m[index * 4 + 1] = col[1];  m[index * 4 + 2] = col[2];  m[index * 4 + 3] = col[3];
}

//inline void Matrix4::SetColumn(int index, const Vector4& v)
//{
//	m[index * 4] = v.x;  m[index * 4 + 1] = v.y;  m[index * 4 + 2] = v.z;  m[index * 4 + 3] = v.w;
//}
//
//inline void Matrix4::SetColumn(int index, const Vector3& v)
//{
//	m[index * 4] = v.x;  m[index * 4 + 1] = v.y;  m[index * 4 + 2] = v.z;
//}

inline const float* Matrix4::Get() const
{
	return m;
}

inline const float* Matrix4::GetTranspose()
{
	tm[0] = m[0];   tm[1] = m[4];   tm[2] = m[8];   tm[3] = m[12];
	tm[4] = m[1];   tm[5] = m[5];   tm[6] = m[9];   tm[7] = m[13];
	tm[8] = m[2];   tm[9] = m[6];   tm[10] = m[10];  tm[11] = m[14];
	tm[12] = m[3];   tm[13] = m[7];   tm[14] = m[11];  tm[15] = m[15];
	return tm;
}

inline Matrix4& Matrix4::Identity()
{
	m[0] = m[5] = m[10] = m[15] = 1.0f;
	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	return *this;
}

inline Matrix4 Matrix4::operator+(const Matrix4& rhs) const
{
	return Matrix4(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2], m[3] + rhs[3],
		m[4] + rhs[4], m[5] + rhs[5], m[6] + rhs[6], m[7] + rhs[7],
		m[8] + rhs[8], m[9] + rhs[9], m[10] + rhs[10], m[11] + rhs[11],
		m[12] + rhs[12], m[13] + rhs[13], m[14] + rhs[14], m[15] + rhs[15]);
}

inline Matrix4 Matrix4::operator-(const Matrix4& rhs) const
{
	return Matrix4(m[0] - rhs[0], m[1] - rhs[1], m[2] - rhs[2], m[3] - rhs[3],
		m[4] - rhs[4], m[5] - rhs[5], m[6] - rhs[6], m[7] - rhs[7],
		m[8] - rhs[8], m[9] - rhs[9], m[10] - rhs[10], m[11] - rhs[11],
		m[12] - rhs[12], m[13] - rhs[13], m[14] - rhs[14], m[15] - rhs[15]);
}

inline Matrix4& Matrix4::operator+=(const Matrix4& rhs)
{
	m[0] += rhs[0];   m[1] += rhs[1];   m[2] += rhs[2];   m[3] += rhs[3];
	m[4] += rhs[4];   m[5] += rhs[5];   m[6] += rhs[6];   m[7] += rhs[7];
	m[8] += rhs[8];   m[9] += rhs[9];   m[10] += rhs[10];  m[11] += rhs[11];
	m[12] += rhs[12];  m[13] += rhs[13];  m[14] += rhs[14];  m[15] += rhs[15];
	return *this;
}

inline Matrix4& Matrix4::operator-=(const Matrix4& rhs)
{
	m[0] -= rhs[0];   m[1] -= rhs[1];   m[2] -= rhs[2];   m[3] -= rhs[3];
	m[4] -= rhs[4];   m[5] -= rhs[5];   m[6] -= rhs[6];   m[7] -= rhs[7];
	m[8] -= rhs[8];   m[9] -= rhs[9];   m[10] -= rhs[10];  m[11] -= rhs[11];
	m[12] -= rhs[12];  m[13] -= rhs[13];  m[14] -= rhs[14];  m[15] -= rhs[15];
	return *this;
}

//inline Vector4 Matrix4::operator*(const Vector4& rhs) const
//{
//	return Vector4(m[0] * rhs.x + m[4] * rhs.y + m[8] * rhs.z + m[12] * rhs.w,
//		m[1] * rhs.x + m[5] * rhs.y + m[9] * rhs.z + m[13] * rhs.w,
//		m[2] * rhs.x + m[6] * rhs.y + m[10] * rhs.z + m[14] * rhs.w,
//		m[3] * rhs.x + m[7] * rhs.y + m[11] * rhs.z + m[15] * rhs.w);
//}
//
//inline Vector3 Matrix4::operator*(const Vector3& rhs) const
//{
//	return Vector3(m[0] * rhs.x + m[4] * rhs.y + m[8] * rhs.z,
//		m[1] * rhs.x + m[5] * rhs.y + m[9] * rhs.z,
//		m[2] * rhs.x + m[6] * rhs.y + m[10] * rhs.z);
//}

inline Matrix4 Matrix4::operator*(const Matrix4& n) const
{
	return Matrix4(m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3], m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3], m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3], m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3],
		m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7], m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7], m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7], m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7],
		m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11], m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11], m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11], m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11],
		m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15], m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15], m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15], m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15]);
}

inline Matrix4& Matrix4::operator*=(const Matrix4& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline bool Matrix4::operator==(const Matrix4& n) const
{
	return (m[0] == n[0]) && (m[1] == n[1]) && (m[2] == n[2]) && (m[3] == n[3]) &&
		(m[4] == n[4]) && (m[5] == n[5]) && (m[6] == n[6]) && (m[7] == n[7]) &&
		(m[8] == n[8]) && (m[9] == n[9]) && (m[10] == n[10]) && (m[11] == n[11]) &&
		(m[12] == n[12]) && (m[13] == n[13]) && (m[14] == n[14]) && (m[15] == n[15]);
}

inline bool Matrix4::operator!=(const Matrix4& n) const
{
	return (m[0] != n[0]) || (m[1] != n[1]) || (m[2] != n[2]) || (m[3] != n[3]) ||
		(m[4] != n[4]) || (m[5] != n[5]) || (m[6] != n[6]) || (m[7] != n[7]) ||
		(m[8] != n[8]) || (m[9] != n[9]) || (m[10] != n[10]) || (m[11] != n[11]) ||
		(m[12] != n[12]) || (m[13] != n[13]) || (m[14] != n[14]) || (m[15] != n[15]);
}

inline float Matrix4::operator[](int index) const
{
	return m[index];
}

inline float& Matrix4::operator[](int index)
{
	return m[index];
}

inline Matrix4 operator-(const Matrix4& rhs)
{
	return Matrix4(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8], -rhs[9], -rhs[10], -rhs[11], -rhs[12], -rhs[13], -rhs[14], -rhs[15]);
}

inline Matrix4 operator*(float s, const Matrix4& rhs)
{
	return Matrix4(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3], s*rhs[4], s*rhs[5], s*rhs[6], s*rhs[7], s*rhs[8], s*rhs[9], s*rhs[10], s*rhs[11], s*rhs[12], s*rhs[13], s*rhs[14], s*rhs[15]);
}

//inline Vector4 operator*(const Vector4& v, const Matrix4& m)
//{
//	return Vector4(v.x*m[0] + v.y*m[1] + v.z*m[2] + v.w*m[3], v.x*m[4] + v.y*m[5] + v.z*m[6] + v.w*m[7], v.x*m[8] + v.y*m[9] + v.z*m[10] + v.w*m[11], v.x*m[12] + v.y*m[13] + v.z*m[14] + v.w*m[15]);
//}
//
//inline Vector3 operator*(const Vector3& v, const Matrix4& m)
//{
//	return Vector3(v.x*m[0] + v.y*m[1] + v.z*m[2], v.x*m[4] + v.y*m[5] + v.z*m[6], v.x*m[8] + v.y*m[9] + v.z*m[10]);
//}

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
//Matrix4& Matrix4::InvertAffine()
//{
//	// R^-1
//	Matrix3 r(m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9], m[10]);
//	r.Invert();
//	m[0] = r[0];  m[1] = r[1];  m[2] = r[2];
//	m[4] = r[3];  m[5] = r[4];  m[6] = r[5];
//	m[8] = r[6];  m[9] = r[7];  m[10] = r[8];
//
//	// -R^-1 * T
//	float x = m[12];
//	float y = m[13];
//	float z = m[14];
//	m[12] = -(r[0] * x + r[3] * y + r[6] * z);
//	m[13] = -(r[1] * x + r[4] * y + r[7] * z);
//	m[14] = -(r[2] * x + r[5] * y + r[8] * z);
//
//	// last row should be unchanged (0,0,0,1)
//	//m[3] = m[7] = m[11] = 0.0f;
//	//m[15] = 1.0f;
//
//	return *this;
//}



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

	float cofactor8 =  GetCofactor(m[1], m[2], m[3], m[5], m[6], m[7], m[13], m[14], m[15]);
	float cofactor9 =  GetCofactor(m[0], m[2], m[3], m[4], m[6], m[7], m[12], m[14], m[15]);
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
float Matrix4::GetCofactor(float m0, float m1, float m2,
						   float m3, float m4, float m5,
						   float m6, float m7, float m8)
{
	return m0 * (m4 * m8 - m5 * m7) -
		   m1 * (m3 * m8 - m5 * m6) +
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
	float m0 = m[0], m2 = m[2],
		m4 = m[4], m6 = m[6],
		m8 = m[8], m10 = m[10],
		m12 = m[12], m14 = m[14];

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
