#pragma once

#include <memory>

class IOwnable
{
public:
	typedef std::shared_ptr<IOwnable> Ptr;

	virtual void SetOwnerIndex(size_t) = 0;
	virtual size_t GetOwnerIndex() = 0;
};
