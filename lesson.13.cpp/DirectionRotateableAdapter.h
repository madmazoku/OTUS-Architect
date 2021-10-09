#pragma once

#include "../lesson.03.cpp/UObject.h"
#include "IDirectionRotateable.h"

class DirectionRotateableAdapter : public IDirectionRotateable
{
protected:
	UObject::Ptr m_pUObject;

public:
	DirectionRotateableAdapter(UObject::Ptr pUObject) : m_pUObject(pUObject) {}

	virtual Vector GetDirection() const override {
		return std::any_cast<Vector>(m_pUObject->GetProperty("direction"));
	}
	virtual void SetDirection(const Vector& direction) override {
		double length = !direction;
		if (length == 0)
			throw std::invalid_argument("direction");
		m_pUObject->SetProperty("direction", direction / length);
	}

	virtual double GetAngleVelocity() const override {
		return std::any_cast<double>(m_pUObject->GetProperty("angleVelocity"));
	}
	virtual void SetAngleVelocity(double angleVelocity) override {
		m_pUObject->SetProperty("angleVelocity", angleVelocity);
	}
};
