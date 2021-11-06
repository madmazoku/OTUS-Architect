#pragma once

#include <utility>

#include "IFuelable.h"
#include "../lesson.03.cpp/IExecuteable.h"
#include "CommandException.h"

class CheckFuelCommand : public IExecuteable
{
protected:
	IFuelable::Ptr m_pFuelable;

public:
	CheckFuelCommand(IFuelable::Ptr pFuelable) : m_pFuelable(std::move(pFuelable)) {}

	void Execute() override {
		if (m_pFuelable->GetFuelLevel() < m_pFuelable->GetFuelCost())
			throw CommandException("fuelLevel");
	}
};
