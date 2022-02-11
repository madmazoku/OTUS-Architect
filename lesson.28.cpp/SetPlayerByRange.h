#pragma once

#include "ISetPlayer.h"

#include "../lesson.03.cpp/IExecuteable.h"
#include "IoC.h"

class SetPlayerByRange : public ISetPlayer
{
protected:
	size_t m_range;
	size_t m_counter;

public:
	SetPlayerByRange(size_t range) : m_range(range), m_counter(0) {
	}

	virtual void SetPlayer(UObject::Ptr pUObject) override {
		size_t ownerIdx = m_counter++ < m_range ? 0 : 1;
		IoC->Resolve<IExecuteable::Ptr>("Tank.SetPlayer", { pUObject, std::to_string(ownerIdx) })->Execute();
	}
};
