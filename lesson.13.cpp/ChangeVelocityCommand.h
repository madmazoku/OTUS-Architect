#pragma once

#include <utility>

#include "../lesson.03.cpp/IExecuteable.h"
#include "../lesson.03.cpp/IMoveable.h"
#include "IDirectionRotateable.h"

class ChangeVelocityCommand : public IExecuteable
{
protected:
	IMoveable::Ptr m_pMovable;
	IDirectionRotateable::Ptr m_pDirectionRotateable;

public:
	ChangeVelocityCommand(
		IMoveable::Ptr pMovable,
		IDirectionRotateable::Ptr pDirectionRotateable
	) :
		m_pMovable(std::move(pMovable)),
		m_pDirectionRotateable(std::move(pDirectionRotateable))
	{}

	virtual void Execute() override {
		m_pMovable->SetVelocity(m_pDirectionRotateable->GetDirection() * m_pMovable->GetVelocity().Length());
	}
};
