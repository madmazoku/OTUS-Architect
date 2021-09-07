#pragma once

#include "Executable.h"
#include "Positional.h"
#include "MoveByVector.h"

class Projectile : public Positional, public Executable
{
public:
	MoveByVector m_move;
	Projectile(const Vector& pos, const Vector& vel) : Positional(pos), m_move(*this, vel) {}

	void Execute(double step) override { m_move.Execute(step); }
};
