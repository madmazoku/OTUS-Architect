#pragma once

#include <string>

#include "ISetPosition.h"

#include "IoC.h"

#include "IGetPlayer.h"

#include "../lesson.02.cpp/Vector.h"

class SetPositionByOffset : public ISetPosition
{
protected:
	Vector m_pos1;
	Vector m_pos2;
	Vector m_off;

	IGetPlayer::Ptr m_pGetPlayer;

public:
	SetPositionByOffset(Vector pos1, Vector pos2, Vector off) : m_pos1(pos1), m_pos2(pos2), m_off(off) {
		m_pGetPlayer = IoC->Resolve<IGetPlayer::Ptr>("Tank.GetterPlayer");
	}

	virtual void SetPosition(UObject::Ptr pUObject) override {
		size_t ownerIdx = m_pGetPlayer->Get(pUObject);
		Vector& pos = (ownerIdx == 0 ? m_pos1 : m_pos2);
		IoC->Resolve<IExecuteable::Ptr>("Tank.SetPosition", { pUObject, std::to_string(pos.m_x), std::to_string(pos.m_y) })->Execute();
		pos += m_off;
	}
};
