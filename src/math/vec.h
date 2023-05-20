#pragma once
#include <stdint.h>
#include <math.h>

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
		Vector(U val = U(0)) : x(T(val)), y(T(val)) {}

		template <typename U>
		Vector(U x, U y) : x(T(x)), y(T(y)) {}

		template <typename U>
		Vector(const Vector<2, U>& other) : x(T(other.x)), y(T(other.y)) {}
	};

	template <size_t L, typename T, typename U>
	Vector<L, T> operator+(const Vector<L, T>& left, const Vector<L, U>& right)
	{
		Vector<L, T> res;
		for (size_t i = 0; i < L; i++)
			res.e[i] = left.e[i] + right.e[i];
		return res;
	}

	template <size_t L, typename T, typename U>
	Vector<L, T> operator-(const Vector<L, T>& left, const Vector<L, U>& right)
	{
		Vector<L, T> res;
		for (size_t i = 0; i < L; i++)
			res.e[i] = left.e[i] - right.e[i];
		return res;
	}

	template <size_t L, typename T, typename U>
	Vector<L, T>& operator+=(Vector<L, T>& res, const Vector<L, U>& other)
	{
		for (size_t i = 0; i < L; i++)
			res.e[i] += other.e[i];
		return res;
	}

	template <size_t L, typename T, typename U>
	Vector<L, T>& operator-=(Vector<L, T>& res, const Vector<L, U>& other)
	{
		for (size_t i = 0; i < L; i++)
			res.e[i] -= other.e[i];
		return res;
	}

	template <size_t L, typename T, typename U>
	Vector<L, T> operator*(const Vector<L, T>& left, const Vector<L, U>& right)
	{
		Vector<L, T> res;
		for (size_t i = 0; i < L; i++)
			res.e[i] = left.e[i] * right.e[i];
		return res;
	}

	template <size_t L, typename T>
	Vector<L, T> operator*(const Vector<L, T>& vector, float scalar)
	{
		Vector<L, T> res;
		for (size_t i = 0; i < L; i++)
			res.e[i] = vector.e[i] * scalar;
		return res;
	}

	template <size_t L, typename T, typename U>
	Vector<L, T> operator/(const Vector<L, T>& left, const Vector<L, U>& right)
	{
		Vector<L, T> res;
		for (size_t i = 0; i < L; i++)
			res.e[i] = left.e[i] / right.e[i];
		return res;
	}

	template <size_t L, typename T>
	Vector<L, T> operator/(const Vector<L, T>& vector, float scalar)
	{
		Vector<L, T> res;
		float r_scalar = 1.f / scalar;
		for (size_t i = 0; i < L; i++)
			res.e[i] = vector.e[i] * r_scalar;
		return res;
	}

	template <size_t L, typename T, typename U>
	Vector<L, T>& operator*=(Vector<L, T>& res, const Vector<L, U>& other)
	{
		for (size_t i = 0; i < L; i++)
			res.e[i] *= other.e[i];
		return res;
	}

	template <size_t L, typename T>
	Vector<L, T>& operator*=(Vector<L, T>& res, float sc)
	{
		for (size_t i = 0; i < L; i++)
			res.e[i] *= sc;
		return res;
	}

	template <size_t L, typename T, typename U>
	Vector<L, T>& operator/=(Vector<L, T>& res, const Vector<L, U>& other)
	{
		for (size_t i = 0; i < L; i++)
			res.e[i] /= other.e[i];
		return res;
	}

	template <size_t L, typename T>
	Vector<L, T>& operator/=(Vector<L, T>& res, float sc)
	{
		float rsc = 1.f / sc;
		for (size_t i = 0; i < L; i++)
			res.e[i] *= rsc;
		return res;
	}

	template <size_t L, typename T, typename U>
	bool operator==(const Vector<L, T>& left, const Vector<L, U>& right)
	{
		for (size_t i = 0; i < L; i++)
			if (left.e[i] != right.e[i])
				return false;

		return true;
	}

	template <size_t L, typename T, typename U>
	bool operator!=(const Vector<L, T>& left, const Vector<L, U>& right)
	{
		return !(left == right);
	}

	template <size_t L, typename T>
	T Dot(const Vector<L, T>& left, const Vector<L, T>& right)
	{
		T res = T(0);
		for (size_t i = 0; i < L; i++)
			res += left.e[i] * right.e[i];
		return res;
	}

	template <size_t L, typename T>
	T LengthSquared(const Vector<L, T>& v)
	{
		return Dot(v, v);
	}

	template <size_t L, typename T>
	T Length(const Vector<L, T>& v)
	{
		return (T)sqrt(LengthSquared(v));
	}

	template <size_t L, typename T>
	Vector<L, T> Normalize(const Vector<L, T>& vector)
	{
		return vector / Length(vector);
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