#pragma once

#include <utility>

#include "IFuelable.h"
#include "CheckFuelCommand.h"
#include "CommandException.h"

class BurnFuelCommand : public CheckFuelCommand
{
public:
	BurnFuelCommand(IFuelable::Ptr pFuelable) : CheckFuelCommand(std::move(pFuelable)) {}

	void Execute() override {
		CheckFuelCommand::Execute();
		m_pFuelable->SetFuelLevel(m_pFuelable->GetFuelLevel() - m_pFuelable->GetFuelCost());
	}
};
