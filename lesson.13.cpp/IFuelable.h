#pragma once

#include <memory>

class IFuelable
{
public:
	typedef std::shared_ptr<IFuelable> Ptr;

public:
	virtual unsigned long GetFuelLevel() const = 0;
	virtual void SetFuelLevel(unsigned long) = 0;

	virtual unsigned long GetFuelCost() const = 0;
	virtual void SetFuelCost(unsigned long) = 0;
};
