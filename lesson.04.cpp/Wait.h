#pragma once

#include <thread>
#include <chrono>

#include "../lesson.03.cpp/IExecuteable.h"

class Wait : public IExecuteable
{
protected:
	std::chrono::milliseconds m_sleepMs;

public:
	Wait(unsigned long sleepMs) : m_sleepMs(sleepMs) {}

	virtual void Execute() override {
		std::this_thread::sleep_for(m_sleepMs);
	}
};

