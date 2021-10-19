#pragma once

#include <ostream>

class Vector
{
public:
	double m_x;
	double m_y;

	Vector() : m_x(0), m_y(0) {}
	Vector(double x, double y) : m_x(x), m_y(y) {}

	Vector operator+(const Vector& v) const { return Vector(m_x + v.m_x, m_y + v.m_y); }
	Vector& operator+=(const Vector& v) { m_x += v.m_x; m_y += v.m_y; return *this; }

	Vector operator*(double d) const { return Vector(m_x * d, m_y * d); }

	double Length() const { return sqrt(m_x * m_x + m_y * m_y); }

	Vector operator/(double d) const { return Vector(m_x / d, m_y / d); }

	friend std::ostream& operator<<(std::ostream& os, const Vector& v);
};

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
	os << '{' << v.m_x << ", " << v.m_y << '}';
	return os;
}