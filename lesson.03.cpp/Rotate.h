#pragma once

#include <math.h>

#include "IExecuteable.h"
#include "IRotateable.h"

class Rotate : public IExecuteable
{
protected:
	IRotateable::Ptr m_pRotateable;

public:
	Rotate(IRotateable::Ptr pRotateable) : m_pRotateable(pRotateable) {}

	virtual void Execute() override {
		Vector velocity = m_pRotateable->GetVelocity();
		double angleVelocity = m_pRotateable->GetAngleVelocity();

		double s = sin(angleVelocity);
		double c = cos(angleVelocity);

		double vx = velocity.m_x * c + velocity.m_y * s;
		double vy = velocity.m_y * c - velocity.m_x * s;

		m_pRotateable->SetVelocity({ vx, vy });
	}

};

