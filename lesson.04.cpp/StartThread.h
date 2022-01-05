#pragma once

#include <utility>

#include "IThreadable.h"
#include "../lesson.03.cpp/IExecuteable.h"

#include "IQueue.h"

class StartThread : public IExecuteable
{
protected:
	IThreadable::Ptr m_pThreadable;
	IQueue<IExecuteable::Ptr>::Ptr m_pQueue;

public:
	StartThread(
		IThreadable::Ptr pThreadable,
		IQueue<IExecuteable::Ptr>::Ptr pQueue
	) : m_pThreadable(std::move(pThreadable)), m_pQueue(std::move(pQueue)) {}

#pragma warning( push )
#pragma warning(disable: 4101)

	virtual void Execute() override {
		ExecuteableQueueThread::Ptr pQueueThread = std::make_shared<ExecuteableQueueThread>(m_pQueue);
		m_pThreadable->SetQueueThread(pQueueThread);

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
