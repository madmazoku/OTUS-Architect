#pragma once

#include "ISetDirection.h"

#include "IoC.h"

#include "../lesson.02.cpp/Vector.h"

class SetDirectionByConstant : public ISetDirection
{
protected:
	Vector m_dir;

public:
	SetDirectionByConstant(Vector dir) : m_dir(std::move(dir)) {}

	virtual void SetDirection(UObject::Ptr pUObject) override {
		IoC->Resolve<IExecuteable::Ptr>("Tank.SetDirection", { pUObject, std::to_string(m_dir.m_x), std::to_string(m_dir.m_y) })->Execute();
	}
};
