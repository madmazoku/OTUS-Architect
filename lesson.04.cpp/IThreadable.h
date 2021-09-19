#pragma once

#include <memory>

#include "../lesson.03.cpp/IExecuteable.h"
#include "QueueThread.h"
#include "LockFreeRingArray.h"

typedef QueueThread<IExecuteable::Ptr, LockFreeRingArray> ExecuteableQueue;

class IThreadable
{
public:
	typedef std::shared_ptr<IThreadable> Ptr;

public:
	virtual ExecuteableQueue::Ptr GetQueue() = 0;
	virtual void SetQueue(ExecuteableQueue::Ptr) = 0;
	virtual void RemoveQueue() = 0;
};
