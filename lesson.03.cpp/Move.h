#pragma once

#include "IExecuteable.h"
#include "IMoveable.h"

class Move : public IExecuteable
{
protected:
	IMoveable::Ptr m_pMoveable;

public:
	Move(IMoveable::Ptr pMoveable) : m_pMoveable(pMoveable) {}

	virtual void Execute() override {
		m_pMoveable->SetPosition(m_pMoveable->GetPosition() + m_pMoveable->GetVelocity());
	}
};

