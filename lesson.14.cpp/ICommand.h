#pragma once

#include <memory>

// Интерфейс команды для использования с фабрикой
class ICommand
{
public:
	typedef std::shared_ptr<ICommand> Ptr;

	virtual void Do() = 0;
};
