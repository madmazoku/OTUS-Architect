#pragma once

#include <functional>

#include "../lesson.03.cpp/IExecuteable.h"

class LambdaCommand : public IExecuteable
{
public:
	typedef std::function<void(void)> Lambda;

protected:
	Lambda m_lambda;

public:
	LambdaCommand(Lambda lambda) : m_lambda(std::move(lambda)) {}

	virtual void Execute() override { m_lambda(); }
};
