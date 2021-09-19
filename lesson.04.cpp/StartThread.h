#pragma once

#include "IThreadable.h"
#include "../lesson.03.cpp/IExecuteable.h"

class StartThread : public IExecuteable
{
protected:
	IThreadable::Ptr m_threadable;

public:
	StartThread(IThreadable::Ptr threadable) : m_threadable(threadable) {}

#pragma warning( push )
#pragma warning(disable: 4101)

	void Execute() override {
		ExecuteableQueue::Ptr pQueue = std::make_shared<ExecuteableQueue>();
		m_threadable->SetQueue(pQueue);

		pQueue->Run([](IExecuteable::Ptr pComand) { 
			try {
				pComand->Execute();
			}
			catch(...) {
				;
			}
		});
		pQueue->Start();
	}

#pragma warning( pop )
};
