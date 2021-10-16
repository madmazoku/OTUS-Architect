#pragma once

#include <memory>

class ICommand
{
public:
	typedef std::shared_ptr<ICommand> Ptr;

	virtual void Do() = 0;
};
