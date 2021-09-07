#pragma once

#include "Positional.h"
#include "Executable.h"

class MoveByVector : public Executable
{
public:
	Positional& m_object;
	Vector m_vel;

	MoveByVector(Positional& object) : m_object(object), m_vel() {}
	MoveByVector(Positional& object, const Vector& vel) : m_object(object), m_vel(vel) {}

	virtual void Execute(double step) override { m_object.m_pos += m_vel * step; }
};
