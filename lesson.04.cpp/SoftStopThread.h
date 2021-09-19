#pragma once

#include "IThreadable.h"
#include "../lesson.03.cpp/IExecuteable.h"

class SoftStopThread : public IExecuteable
{
protected:
	IThreadable::Ptr m_threadable;

public:
	SoftStopThread(IThreadable::Ptr threadable) : m_threadable(threadable) {}

	void Execute() override {
		ExecuteableQueueThread::Ptr pQueue = m_threadable->GetQueueThread();
		pQueue->SoftStop();
	}
};
