#pragma once
#include <cmath>

#include "../lesson.03.cpp/IExecuteable.h"
#include "IDirectionRotateable.h"

class DirectionRotate : public IExecuteable
{
protected:
	IDirectionRotateable::Ptr m_pDirectionRotateable;

public:
	DirectionRotate(IDirectionRotateable::Ptr pDirectionRotateable) : m_pDirectionRotateable(pDirectionRotateable) {}

	virtual void Execute() override {
		Vector direction = m_pDirectionRotateable->GetDirection();
		double angleVelocity = m_pDirectionRotateable->GetAngleVelocity();

		double s = sin(angleVelocity);
		double c = cos(angleVelocity);

		double vx = direction.m_x * c + direction.m_y * s;
		double vy = direction.m_y * c - direction.m_x * s;

		m_pDirectionRotateable->SetDirection({ vx, vy });
	}
};
