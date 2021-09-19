#pragma once

#include <any>

#include "IThreadable.h"
#include "../lesson.03.cpp/UObject.h"

class ThreadableAdapter : public IThreadable
{
protected:
	UObject::Ptr m_pUObject;

public:
	ThreadableAdapter(UObject::Ptr pUObject) : m_pUObject(pUObject) {}

	virtual ExecuteableQueueThread::Ptr GetQueueThread() override {
		return std::any_cast<ExecuteableQueueThread::Ptr>(m_pUObject->GetProperty("executionableQueueThread"));
	}
	virtual void SetQueueThread(ExecuteableQueueThread::Ptr pExecutionableQueue) override {
		m_pUObject->SetProperty("executionableQueueThread", pExecutionableQueue);
	}
	virtual void RemoveQueueThread() override {
		m_pUObject->RemoveProperty("executionableQueueThread");
	}
};
