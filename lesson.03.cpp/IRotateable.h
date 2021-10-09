#pragma once

#include "../lesson.02.cpp/Vector.h"
#include <memory>

class IRotateable
{
public:
	typedef std::shared_ptr<IRotateable> Ptr;

public:

	virtual Vector GetVelocity() const = 0;
	virtual void SetVelocity(const Vector& velocity) = 0;

	virtual double GetAngleVelocity() const = 0;
	virtual void SetAngleVelocity(double) = 0;
};
