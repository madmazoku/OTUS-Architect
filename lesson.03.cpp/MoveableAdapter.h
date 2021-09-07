#pragma once

#include "IMoveable.h"
#include "UObject.h"

class MoveableAdapter : public IMoveable
{
protected:
	UObject::Ptr m_pUObject;

public:
	MoveableAdapter(UObject::Ptr pUObject) : m_pUObject(pUObject) {}

	virtual Vector GetPosition() const override {
		return std::any_cast<Vector>(m_pUObject->GetProperty("position"));
	}
	virtual void SetPosition(const Vector& position) override {
		m_pUObject->SetProperty("position", position);
	}

	virtual Vector GetVelocity() const override {
		return std::any_cast<Vector>(m_pUObject->GetProperty("velocity"));
	}
	virtual void SetVelocity(const Vector& velocity) override {
		m_pUObject->SetProperty("velocity", velocity);
	}
};
