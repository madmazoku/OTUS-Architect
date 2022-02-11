#pragma once

#include "../lesson.03.cpp/UObject.h"

class IGetPlayer
{
public:
	typedef std::shared_ptr<IGetPlayer> Ptr;

	virtual size_t Get(UObject::Ptr) = 0;
};
