#pragma once

#include <memory>

#include "../Common/Vector.h"

class IMoveable
{
public:
	typedef std::shared_ptr<IMoveable> Ptr;

public:
	virtual Vector GetPosition() const = 0;
	virtual void SetPosition(const Vector& position) = 0;

	virtual Vector GetVelocity() const = 0;
	virtual void SetVelocity(const Vector& velocity) = 0;
};

