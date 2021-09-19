#pragma once

#include <memory>

#include "../lesson.03.cpp/IExecuteable.h"
#include "QueueThread.h"
#include "LockFreeRingArray.h"

typedef QueueThread<IExecuteable::Ptr> ExecuteableQueueThread;

class IThreadable
{
public:
	typedef std::shared_ptr<IThreadable> Ptr;

public:
	virtual ExecuteableQueueThread::Ptr GetQueueThread() = 0;
	virtual void SetQueueThread(ExecuteableQueueThread::Ptr) = 0;
	virtual void RemoveQueueThread() = 0;
};
