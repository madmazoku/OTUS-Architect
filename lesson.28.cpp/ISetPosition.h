#pragma once

#include <memory>

#include "../lesson.02.cpp/Vector.h"
#include "../lesson.03.cpp/UObject.h"

class ISetPosition
{
public:
	typedef std::shared_ptr<ISetPosition> Ptr;

	virtual void SetPosition(UObject::Ptr) = 0;
};
