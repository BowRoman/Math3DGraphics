inline Vector4 Vector4::Black()
{
	return Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

inline Vector4 Vector4::White()
{
	return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

inline Vector4 Vector4::Gray()
{
	return Vector4(0.5f, 0.5f, 0.5f, 1.0f);
}

inline Vector4 Vector4::Red()
{
	return Vector4(1.0f, 0.0f, 0.0f, 1.0f);
}

inline Vector4 Vector4::Green()
{
	return Vector4(0.0f, 1.0f, 0.0f, 1.0f);
}

inline Vector4 Vector4::Blue()
{
	return Vector4(0.0f, 0.0f, 1.0f, 1.0f);
}

inline Vector4 Vector4::Yellow()
{
	return Vector4(1.0f, 1.0f, 0.0f, 1.0f);
}

inline Vector4 Vector4::Magenta()
{
	return Vector4(1.0f, 0.0f, 1.0f, 1.0f);
}

inline Vector4 Vector4::Cyan()
{
	return Vector4(0.0f, 1.0f, 1.0f, 1.0f);
}

inline Vector4 Vector4::Orange()
{
	return Vector4(1.0f, 0.5f, 0.0f, 1.0f);
}

inline Vector4 Vector4::Lime()
{
	return Vector4(0.5f, 1.0f, 0.0f, 1.0f);
}

inline Vector4 Vector4::CyanGreen()
{
	return Vector4(0.0f, 1.0f, 0.5f, 1.0f);
}

inline Vector4 Vector4::Azure()
{
	return Vector4(0.0f, 0.5f, 1.0f, 1.0f);
}

inline Vector4 Vector4::Violet()
{
	return Vector4(0.5f, 0.0f, 1.0f, 1.0f);
}

inline Vector4 Vector4::Strobe()
{
	return Vector4(Math::Random::GetF(0.0f, 1.0f), Math::Random::GetF(0.0f, 1.0f), Math::Random::GetF(0.0f, 1.0f), 1.0f);
}

inline Vector4 Vector4::Rose()
{
	return Vector4(1.0f, 0.0f, 0.5f, 1.0f);
}

inline Vector4 Vector4::operator*(float s) const
{
	return Vector4(x * s, y * s, z * s, w * s);
}