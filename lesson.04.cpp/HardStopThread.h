#pragma once

#include <utility>

#include "IThreadable.h"
#include "../lesson.03.cpp/IExecuteable.h"

class HardStopThread : public IExecuteable
{
protected:
	IThreadable::Ptr m_pThreadable;

public:
	HardStopThread(IThreadable::Ptr pThreadable) : m_pThreadable(std::move(pThreadable)) {}

	virtual void Execute() override {
		ExecuteableQueueThread::Ptr pQueue = m_pThreadable->GetQueueThread();
		pQueue->HardStop();
	}
};
