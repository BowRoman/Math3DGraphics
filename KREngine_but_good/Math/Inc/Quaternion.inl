inline Quaternion Quaternion::Zero()
{
	return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}

inline Quaternion Quaternion::Identity()
{
	return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

inline Quaternion Quaternion::operator+(const Quaternion& rhs) const
{
	return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

inline Quaternion Quaternion::operator*(float s) const
{
	return Quaternion(x * s, y * s, z * s, w * s);
}