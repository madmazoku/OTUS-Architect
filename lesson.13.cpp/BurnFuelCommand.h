#pragma once

#include "IFuelable.h"
#include "CheckFuelCommand.h"
#include "CommandException.h"

class BurnFuelCommand : public CheckFuelCommand
{
protected:
	IFuelable::Ptr m_pFuelable;

public:
	BurnFuelCommand(IFuelable::Ptr pFuelable) : CheckFuelCommand(pFuelable), m_pFuelable(pFuelable) {}

	void Execute() override {
		CheckFuelCommand::Execute();
		m_pFuelable->SetFuelLevel(m_pFuelable->GetFuelLevel() - m_pFuelable->GetFuelCost());
	}
};
