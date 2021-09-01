#pragma once

#include "Positional.h"
#include "Directional.h"
#include "MoveByVector.h"
#include "RotateByAngle.h"
#include "Cannon.h"

class Tank : public Positional, public Directional, public Executable
{
public:
	MoveByVector m_move;
	RotateByAngle m_rotate;
	Cannon m_cannon;

	Tank(const Vector& pos, const Vector& vel, const Vector& dir, double a, const Vector& cannonDir, double cannonA) 
		: Positional(pos), Directional(dir), m_move(*this, vel), m_rotate(*this, a), m_cannon(*this, cannonDir, cannonA) 
	{}

	void Execute(double step) override { m_rotate.Execute(step); m_move.Execute(step); m_cannon.Execute(step); }
};

