#pragma once

#include <memory>

class IIdentifyable
{
public:
	typedef std::shared_ptr<IIdentifyable> Ptr;

	virtual void SetIndex(size_t) = 0;
	virtual size_t GetIndex() = 0;
};
