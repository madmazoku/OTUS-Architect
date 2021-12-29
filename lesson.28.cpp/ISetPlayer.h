#pragma once

#include <memory>

#include "../lesson.03.cpp/UObject.h"

class ISetPlayer
{
public:
	typedef std::shared_ptr<ISetPlayer> Ptr;

	virtual void SetPlayer(UObject::Ptr pUObject) = 0;
};
