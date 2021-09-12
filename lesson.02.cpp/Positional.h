#pragma once

#include "Vector.h"

class Positional
{
public:
	Vector m_pos;

	Positional() : m_pos() {}
	Positional(const Vector& pos) : m_pos(pos) {}
};
