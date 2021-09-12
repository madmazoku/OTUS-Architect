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

	virtual ExecuteableQueue::Ptr GetQueue() override {
		return std::any_cast<ExecuteableQueue::Ptr>(m_pUObject->GetProperty("executionableQueue"));
	}
	virtual void SetQueue(ExecuteableQueue::Ptr pExecutionableQueue) override {
		m_pUObject->SetProperty("executionableQueue", pExecutionableQueue);
	}
	virtual void RemoveQueue() override {
		m_pUObject->RemoveProperty("executionableQueue");
	}
};

