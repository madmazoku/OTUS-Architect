#pragma once

#include <any>
#include "../lesson.03.cpp/UObject.h"
#include "IFuelable.h"

class FuelableAdapter : public IFuelable
{
protected:
	UObject::Ptr m_pUObject;

public:
	FuelableAdapter(UObject::Ptr pUObject) : m_pUObject(pUObject) {}

	virtual unsigned long GetFuelLevel() const override {
		return std::any_cast<unsigned long>(m_pUObject->GetProperty("fuelLevel"));
	}
	virtual void SetFuelLevel(unsigned long fuelLevel) override {
		m_pUObject->SetProperty("fuelLevel", fuelLevel);
	}

	virtual unsigned long GetFuelCost() const override {
		return std::any_cast<unsigned long>(m_pUObject->GetProperty("fuelCost"));
	}
	virtual void SetFuelCost(unsigned long fuelCost) override {
		m_pUObject->SetProperty("fuelCost", fuelCost);
	}
};
