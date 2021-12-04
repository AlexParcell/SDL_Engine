#pragma once

#include <math.h>

class Vector2
{
public:
	float x;
	float y;

	Vector2()
		: x(0.0f), y(0.0f)
	{

	}

	Vector2(float _x, float _y)
	{
		x = _x; y = _y;
	}

	Vector2(const Vector2& v)
	{
		x = v.x;
		y = v.y;
	}

	Vector2 operator=(const Vector2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	Vector2 operator+(const Vector2& v)
	{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2 operator-(const Vector2& v)
	{
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector2 operator*(const Vector2& v)
	{
		return Vector2(x * v.x, y * v.y);
	}

	Vector2 operator*=(const Vector2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vector2 operator*(const float n)
	{
		return Vector2(x * n, y * n);
	}

	Vector2 operator*=(const float n)
	{
		x *= n;
		y *= n;
		return *this;
	}

	Vector2 operator/(const Vector2& v)
	{
		return Vector2(x / v.x, y / v.y);
	}

	Vector2 operator/=(const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	Vector2 operator/(const float n)
	{
		return Vector2(x / n, y / n);
	}

	Vector2 operator/=(const float n)
	{
		x /= n;
		y /= n;
		return *this;
	}

	float magnitudeSquared()
	{
		return x * x + y * y;
	}

	float magnitude()
	{
		return sqrt(magnitudeSquared());
	}

	Vector2 normalize()
	{
		float mag = magnitude();
		if (mag != 0)
		{
			return Vector2(x/mag, y/mag);
		}
	}

	float dotProduct(const Vector2& v)
	{
		return (x * v.x) + (y * v.y);
	}
};