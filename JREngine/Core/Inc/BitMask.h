#pragma once
#include <type_traits>

/*
This file enables the use of enum classes as bitmasks provided that they
include a variable named "enable_bit_flags"
*/

template<typename Enum>
// if the type does not contain "enable_bit_flags" or is not an enum, this operator should fail
inline typename std::enable_if<std::is_enum<decltype(Enum::enable_bit_flags)>::value, Enum>::type
operator |(Enum lhs, Enum rhs)
{
	using underType = typename std::underlying_type<Enum>::type;

	return static_cast<Enum>
		(static_cast<underType>(lhs)
			| static_cast<underType>(rhs));
}

template<typename Enum>
inline typename std::enable_if<std::is_enum<decltype(Enum::enable_bit_flags)>::value, Enum>::type
operator &(Enum lhs, Enum rhs)
{
	using underType = std::underlying_type<Enum>::type;

	return static_cast<Enum>
		(static_cast<underType>(lhs)
			& static_cast<underType>(rhs));
}

template<typename Enum>
inline typename std::enable_if<std::is_enum<decltype(Enum::enable_bit_flags)>::value, Enum>::type
operator ^(Enum lhs, Enum rhs)
{
	using underType = std::underlying_type<Enum>::type;

	return static_cast<Enum>
		(static_cast<underType>(lhs)
			^ static_cast<underType>(rhs));
}

template<typename Enum>
inline typename std::enable_if<std::is_enum<decltype(Enum::enable_bit_flags)>::value, Enum>::type
operator ~(Enum rhs)
{
	using underType = std::underlying_type<Enum>::type;

	return static_cast<Enum>
		(~static_cast<underType>(rhs));
}

template<typename Enum>
inline typename std::enable_if<std::is_enum<decltype(Enum::enable_bit_flags)>::value, Enum&>::type
operator |=(Enum& lhs, Enum rhs)
{
	using underType = std::underlying_type<Enum>::type;

	lhs = static_cast<Enum>
		(static_cast<underType>(lhs)
			| static_cast<underType>(rhs));

	return lhs;
}

template<typename Enum>
inline typename std::enable_if<std::is_enum<decltype(Enum::enable_bit_flags)>::value, Enum&>::type
operator &=(Enum& lhs, Enum rhs)
{
	using underType = std::underlying_type<Enum>::type;

	lhs = static_cast<Enum>
		(static_cast<underType>(lhs)
			& static_cast<underType>(rhs));

	return lhs;
}

template<typename Enum>
inline typename std::enable_if<std::is_enum<decltype(Enum::enable_bit_flags)>::value, Enum&>::type
operator ^=(Enum& lhs, Enum rhs)
{
	using underType = std::underlying_type<Enum>::type;

	lhs = static_cast<Enum>
		(static_cast<underType>(lhs)
			^ static_cast<underType>(rhs));

	return lhs;
}