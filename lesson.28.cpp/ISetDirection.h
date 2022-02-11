#pragma once

#include <memory>

#include "../lesson.03.cpp/UObject.h"

class ISetDirection
{
public:
	typedef std::shared_ptr<ISetDirection> Ptr;

	virtual void SetDirection(UObject::Ptr) = 0;
};
