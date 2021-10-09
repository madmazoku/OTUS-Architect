#pragma once

#include "../lesson.03.cpp/IExecuteable.h"
#include "../lesson.03.cpp/IMoveable.h"
#include "IDirectionRotateable.h"

class ChangeVelocityCommand : public IExecuteable
{
protected:
	IMoveable::Ptr m_pMovable;
	IDirectionRotateable::Ptr m_pDirectionRotateable;

public:
	ChangeVelocityCommand(IMoveable::Ptr pMovable, IDirectionRotateable::Ptr pDirectionRotateable) : m_pMovable(pMovable), m_pDirectionRotateable(pDirectionRotateable) {}

	virtual void Execute() {
		m_pMovable->SetVelocity(m_pDirectionRotateable->GetDirection() * !m_pMovable->GetVelocity());
	}
};
