#pragma once

#include <functional>

#include "../lesson.03.cpp/UObject.h"
#include "../lesson.03.cpp/IExecuteable.h"

class Generic : public IExecuteable
{
protected:
	UObject::Ptr m_pUObject;
	std::function<void(UObject::Ptr)> m_lambda;

public:
	Generic(UObject::Ptr pUObject, std::function<void(UObject::Ptr)> lambda) : m_pUObject(pUObject), m_lambda(lambda) {}

	virtual void Execute() override {
		m_lambda(m_pUObject);
	}
};
