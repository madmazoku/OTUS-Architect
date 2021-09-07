#pragma once

#include "Directional.h"
#include "RotateByAngle.h"
#include "Projectile.h"
#include "Executable.h"

class Cannon : public Directional, public Executable
{
public:
	Positional& m_owner;
	RotateByAngle m_rotate;

	Cannon(Positional& owner, const Vector& dir, double a) : Directional(dir), m_owner(owner), m_rotate(*this, a) {}

	void Execute(double step) override { m_rotate.Execute(step); }

	Projectile* Shoot() { return new Projectile(m_owner.m_pos, m_dir); }
};
