#pragma once

#include "../lesson.03.cpp/IExecuteable.h"
#include <list>

class MacroCommand : public IExecuteable
{
public:
	typedef std::list<IExecuteable::Ptr> CommandList;
protected:
	CommandList m_commands;

public:
	MacroCommand(const CommandList& commands) : m_commands(commands) {}

	virtual void Execute() override {
		for (IExecuteable::Ptr& command : m_commands)
			command->Execute();
	}
};
