#pragma once
#include <stdint.h>

namespace Math
{
	template <size_t L, typename T>
	struct Vector;

	template <typename T>
	struct Vector<2, T>
	{
		union
		{
			struct { T x, y; };
			struct { T r, g; };
			struct { T u, v; };
			T e[2];
		};

		Vector() : x(T(0)), y(T(0)) {}
		template <typename U>
		Vector(U x, U y) : x(T(x)), y(T(y)) {}
	};

	template <size_t L, typename T>
	Vector<L, T> operator+(const Vector<L, T>& left, const Vector<L, T>& right)
	{
		Vector<L, T> res;
		for (size_t i = 0; i < L; i++)
			res.e[i] = left.e[i] + right.e[i];
		return res;
	}

	template <size_t L, typename T>
	Vector<L, T> operator-(const Vector<L, T>& left, const Vector<L, T>& right)
	{
		Vector<L, T> res;
		for (size_t i = 0; i < L; i++)
			res.e[i] = left.e[i] - right.e[i];
		return res;
	}

	template <size_t L, typename T>
	Vector<L, T>& operator+=(Vector<L, T>& res, const Vector<L, T>& other)
	{
		for (size_t i = 0; i < L; i++)
			res.e[i] += other.e[i];
		return res;
	}

	template <size_t L, typename T>
	Vector<L, T>& operator-=(Vector<L, T>& res, const Vector<L, T>& other)
	{
		for (size_t i = 0; i < L; i++)
			res.e[i] -= other.e[i];
		return res;
	}

	template <size_t L, typename T>
	bool operator==(const Vector<L, T>& left, const Vector<L, T>& right)
	{
		for (size_t i = 0; i < L; i++)
			if (left.e[i] != right.e[i])
				return false;

		return true;
	}

	template <typename T>
	inline constexpr bool is_integer = false;

	template <>
	inline constexpr bool is_integer<int32_t> = true;

	template <typename T>
	inline constexpr bool is_floating_point = false;

	template <>
	inline constexpr bool is_floating_point<float> = true;

	template <typename ostream, size_t L, typename T>
	ostream& operator<<(ostream& stream, const Vector<L, T>& vector)
	{
		stream << "Vec" << L;
		if constexpr (is_integer<T>)
			stream << "i";
		else if constexpr (is_floating_point<T>)
			stream << "f";
		stream << "(";
		for (size_t i = 0; i < L - 1; i++)
			stream << vector.e[i] << ", ";
		stream << vector.e[L - 1] << ")";

		return stream;
	}

	using Vec2f = Vector<2, float>;
	using Vec2i = Vector<2, int32_t>;
}