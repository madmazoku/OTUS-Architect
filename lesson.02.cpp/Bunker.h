#pragma once

#include "Positional.h"
#include "Cannon.h"
#include "Executable.h"

class Bunker : public Positional, public Executable
{
public:
	Cannon m_cannon;

	Bunker(const Vector& pos, const Vector& dir, double a) : Positional(pos), m_cannon(*this, dir, a) {}

	void Execute(double step) { m_cannon.Execute(step); }
};
