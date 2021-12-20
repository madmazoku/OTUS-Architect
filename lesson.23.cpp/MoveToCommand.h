#pragma once

#include "QueueStatefullThread.h"
#include "QueueState.h"
#include "../lesson.03.cpp/IExecuteable.h"

class MoveToCommand : public IExecuteable
{
protected:
	QueueStatefullThread::Ptr m_pQueueThreadFrom;
	QueueStatefullThread::Ptr m_pQueueThreadTo;

public:
	MoveToCommand(QueueStatefullThread::Ptr pQueueThreadFrom, QueueStatefullThread::Ptr pQueueThreadTo) : m_pQueueThreadFrom(std::move(pQueueThreadFrom)), m_pQueueThreadTo(std::move(pQueueThreadTo)) {}

	virtual void Execute() override {
		QueueStatefullThread::Ptr pQueueThreadTo = m_pQueueThreadTo;
		QueueState::Ptr pMoveToQueueState = std::make_shared<QueueState>([pQueueThreadTo](IExecuteable::Ptr pCmd) { pQueueThreadTo->Put(pCmd); });
		pMoveToQueueState->SetNextState(pMoveToQueueState);
		m_pQueueThreadFrom->GetQueueState()->SetNextState(pMoveToQueueState);
	}
};
