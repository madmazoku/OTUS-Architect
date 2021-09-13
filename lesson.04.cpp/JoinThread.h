#pragma once

#include "IThreadable.h"
#include "../lesson.03.cpp/IExecuteable.h"

class JoinThread : public IExecuteable
{
protected:
	IThreadable::Ptr m_threadable;

public:
	JoinThread(IThreadable::Ptr threadable) : m_threadable(threadable) {}

	void Execute() override {
		ExecuteableQueue::Ptr pQueue = m_threadable->GetQueue();
		pQueue->Join();
		m_threadable->RemoveQueue();
	}
};
