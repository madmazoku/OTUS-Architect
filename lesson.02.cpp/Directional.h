#pragma once

#include "Vector.h"

class Directional
{
public:
	Vector m_dir;

	Directional() : m_dir() {}
	Directional(const Vector& dir) : m_dir(dir / dir.Length()) {}
};
