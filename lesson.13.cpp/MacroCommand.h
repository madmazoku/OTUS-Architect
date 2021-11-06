#pragma once

#include <utility>

#include "../lesson.03.cpp/IExecuteable.h"
#include <vector>

class MacroCommand : public IExecuteable
{
public:
	typedef std::vector<IExecuteable::Ptr> Commands;
protected:
	Commands m_commands;

public:
	MacroCommand(Commands commands) : m_commands(std::move(commands)) {}

	virtual void Execute() override {
		for (IExecuteable::Ptr& command : m_commands)
			command->Execute();
	}
};
