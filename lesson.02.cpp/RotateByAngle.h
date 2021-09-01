#pragma once

#include <math.h>

#include "Directional.h"
#include "Executable.h"

class RotateByAngle : public Executable
{
public:
	Directional& m_object;
	double m_a;

	RotateByAngle(Directional& object) : m_object(object), m_a(0) {}
	RotateByAngle(Directional& object, double a) : m_object(object), m_a(a) {}

	virtual void Execute(double step) override {
		double angleFraction = m_a * step;
		double s = sin(angleFraction);
		double c = cos(angleFraction);
		double x = m_object.m_dir.m_x * c + m_object.m_dir.m_y * s;
		double y = m_object.m_dir.m_y * c - m_object.m_dir.m_x * s;
		m_object.m_dir.m_x = x;
		m_object.m_dir.m_y = y;
	};
};

