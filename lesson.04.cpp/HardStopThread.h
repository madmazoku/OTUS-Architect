#pragma once

#include "IThreadable.h"
#include "../lesson.03.cpp/IExecuteable.h"

class HardStopThread : public IExecuteable
{
protected:
	IThreadable::Ptr m_threadable;

public:
	HardStopThread(IThreadable::Ptr threadable) : m_threadable(threadable) {}

	void Execute() override {
		ExecuteableQueue::Ptr pQueue = m_threadable->GetQueue();
		pQueue->HardStop();
	}
};
