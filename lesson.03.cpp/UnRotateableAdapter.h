#pragma once

#include "IRotateable.h"
#include "UObject.h"

class UnRotateableAdapter : public IRotateable
{
protected:
	UObject::Ptr m_pUObject;

public:
	UnRotateableAdapter(UObject::Ptr pUObject) : m_pUObject(pUObject) {}

	virtual Vector GetVelocity() const override {
		return std::any_cast<Vector>(m_pUObject->GetProperty("velocity"));
	}
	virtual void SetVelocity(const Vector& velocity) override {
		throw std::invalid_argument("velocity");
	}

	virtual double GetAngleVelocity() const override {
		return std::any_cast<double>(m_pUObject->GetProperty("angleVelocity"));
	}
	virtual void SetAngleVelocity(double angleVelocity) override {
		throw std::invalid_argument("angleVelocity");
	}
};