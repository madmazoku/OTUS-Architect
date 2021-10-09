#pragma once

#include "IRotateable.h"
#include "UObject.h"

class RotateableAdapter : public IRotateable
{
protected:
	UObject::Ptr m_pUObject;

public:
	RotateableAdapter(UObject::Ptr pUObject) : m_pUObject(pUObject) {}

	virtual Vector GetVelocity() const override {
		return std::any_cast<Vector>(m_pUObject->GetProperty("velocity"));
	}
	virtual void SetVelocity(const Vector& velocity) override {
		m_pUObject->SetProperty("velocity", velocity);
	}

	virtual double GetAngleVelocity() const override {
		return std::any_cast<double>(m_pUObject->GetProperty("angleVelocity"));
	}
	virtual void SetAngleVelocity(double angleVelocity) override {
		m_pUObject->SetProperty("angleVelocity", angleVelocity);
	}
};
