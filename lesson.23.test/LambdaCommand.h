#pragma once

#include <functional>

#include "../lesson.03.cpp/IExecuteable.h"

class LambdaCommand : public IExecuteable
{
protected:
	std::function<void()> m_lambda;

public:
	LambdaCommand(std::function<void()> lambda) : m_lambda(lambda) {}

	virtual void Execute() override {
		m_lambda();
	}
};
