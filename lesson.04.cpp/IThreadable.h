#pragma once

#include <memory>

#include "../lesson.03.cpp/IExecuteable.h"
#include "QueueThread.h"
#include "TwoLockQueue.h"

typedef QueueThread<IExecuteable::Ptr, TwoLockQueue> ExecuteableQueue;

class IThreadable
{
public:
	typedef std::shared_ptr<IThreadable> Ptr;

public:
	virtual ExecuteableQueue::Ptr GetQueue() = 0;
	virtual void SetQueue(ExecuteableQueue::Ptr) = 0;
	virtual void RemoveQueue() = 0;
};
