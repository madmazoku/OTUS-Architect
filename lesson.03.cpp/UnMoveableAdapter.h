#pragma once

#include <stdexcept>

#include "IMoveable.h"
#include "UObject.h"

class UnMoveableAdapter : public IMoveable
{
protected:
	UObject::Ptr m_pUObject;

public:
	UnMoveableAdapter(UObject::Ptr pUObject) : m_pUObject(pUObject) {}

	virtual Vector GetPosition() const override {
		return std::any_cast<Vector>(m_pUObject->GetProperty("position"));
	}
	virtual void SetPosition(const Vector& position) override {
		throw std::invalid_argument("position");
	}

	virtual Vector GetVelocity() const override {
		return std::any_cast<Vector>(m_pUObject->GetProperty("velocity"));
	}
	virtual void SetVelocity(const Vector& velocity) override {
		m_pUObject->SetProperty("velocity", velocity);
	}
};

