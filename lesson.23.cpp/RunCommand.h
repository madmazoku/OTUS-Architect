#pragma once

#include "QueueStatefullThread.h"
#include "QueueState.h"
#include "../lesson.03.cpp/IExecuteable.h"

// команда смены состояния обработчика команд в очереди на их выполнение
class RunCommand : public IExecuteable
{
protected:
	QueueStatefullThread::Ptr m_pQueueThread;

public:
	RunCommand(QueueStatefullThread::Ptr pQueueThread) : m_pQueueThread(std::move(pQueueThread)) {}

	virtual void Execute() override {
		QueueState::Ptr pRunQueueState = std::make_shared<QueueState>([](IExecuteable::Ptr pCmd) { pCmd->Execute(); });
		pRunQueueState->SetNextState(pRunQueueState);
		m_pQueueThread->GetQueueState()->SetNextState(pRunQueueState);
	}
};
