#pragma once

#include <memory>

class IExecuteable
{
public:
	typedef std::shared_ptr<IExecuteable> Ptr;

public:
	virtual void Execute() = 0;
};

