#pragma once

#include <utility>

#include "IThreadable.h"
#include "../lesson.03.cpp/IExecuteable.h"

class JoinThread : public IExecuteable
{
protected:
	IThreadable::Ptr m_pThreadable;

public:
	JoinThread(IThreadable::Ptr pThreadable) : m_pThreadable(std::move(pThreadable)) {}

	void Execute() override {
		ExecuteableQueueThread::Ptr pQueue = m_pThreadable->GetQueueThread();
		pQueue->Join();
		m_pThreadable->RemoveQueueThread();
	}
};
