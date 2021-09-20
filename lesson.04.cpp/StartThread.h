#pragma once

#include "IThreadable.h"
#include "../lesson.03.cpp/IExecuteable.h"

#include "IQueue.h"

class StartThread : public IExecuteable
{
protected:
	IThreadable::Ptr m_threadable;
	IQueue<IExecuteable::Ptr>::Ptr m_pQueue;

public:
	StartThread(
		IThreadable::Ptr threadable,
		IQueue<IExecuteable::Ptr>::Ptr pQueue
	) : m_threadable(threadable), m_pQueue(pQueue) {}

#pragma warning( push )
#pragma warning(disable: 4101)

	void Execute() override {
		ExecuteableQueueThread::Ptr pQueueThread = std::make_shared<ExecuteableQueueThread>(m_pQueue);
		m_threadable->SetQueueThread(pQueueThread);

		pQueueThread->Run([](IExecuteable::Ptr pComand) {
			try {
				pComand->Execute();
			}
			catch (...) {
				;
			}
			});
		pQueueThread->Start();
	}

#pragma warning( pop )
};
