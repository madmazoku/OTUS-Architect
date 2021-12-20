#pragma once

#include "QueueStatefullThread.h"
#include "../lesson.03.cpp/IExecuteable.h"

#include <iostream>

class HardStopCommand : public IExecuteable
{
protected:
	QueueStatefullThread::Ptr m_pQueueThread;

public:
	HardStopCommand(QueueStatefullThread::Ptr pQueueThread) : m_pQueueThread(std::move(pQueueThread)) {}

	virtual void Execute() override {
		m_pQueueThread->GetQueueState()->SetNextState(nullptr);
	}
};
