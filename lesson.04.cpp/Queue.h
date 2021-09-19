#pragma once

#include <memory>

template<class T>
class IQueue {
public:
	typedef std::shared_ptr<IQueue> Ptr;

public:
	virtual bool Put(T) = 0;
	virtual bool Get(T&) = 0;
};
