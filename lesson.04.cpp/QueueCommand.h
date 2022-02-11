#pragma once

#include <utility>

#include "IThreadable.h"
#include "../lesson.03.cpp/IExecuteable.h"

class QueueCommand : public IExecuteable
{
protected:
	IThreadable::Ptr m_pThreadable;
	IExecuteable::Ptr m_pCommand;

public:

	QueueCommand(IThreadable::Ptr pThreadable, IExecuteable::Ptr command) : m_pThreadable(std::move(pThreadable)), m_pCommand(std::move(command)) {}

	virtual void Execute() override {
		ExecuteableQueueThread::Ptr queue = m_pThreadable->GetQueueThread();
		queue->Put(m_pCommand);
	}
};
