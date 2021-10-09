#pragma once

#include <memory>

#include "../lesson.02.cpp/Vector.h"

class IDirectionRotateable
{
public:
	typedef std::shared_ptr<IDirectionRotateable> Ptr;

public:

	virtual Vector GetDirection() const = 0;
	virtual void SetDirection(const Vector&) = 0;

	virtual double GetAngleVelocity() const = 0;
	virtual void SetAngleVelocity(double) = 0;
};
