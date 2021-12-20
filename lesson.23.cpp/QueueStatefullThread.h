#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iostream>

#include "QueueState.h"

#include "../lesson.04.cpp/IQueue.h"
#include "../lesson.03.cpp/IExecuteable.h"

typedef IQueue<IExecuteable::Ptr> IExecuteableQueue;

class QueueStatefullThread
{
public:
	typedef std::shared_ptr<QueueStatefullThread> Ptr;

protected:
	IExecuteableQueue::Ptr m_pQueue;
	QueueState::Ptr m_pQueueState;

	enum class Status {
		Initial,
		Running,
		Stopped,
	};
	Status m_status;

	std::mutex m_lock;
	std::condition_variable m_cv;
	std::thread m_thread;

public:
	QueueStatefullThread(IExecuteableQueue::Ptr pQueue, QueueState::Ptr pInitialQueueState) : m_pQueue(std::move(pQueue)), m_pQueueState(std::move(pInitialQueueState)), m_status(Status::Initial) {}

	void Put(IExecuteable::Ptr cmd) {
		while (m_status != Status::Stopped && !m_pQueue->Put(cmd)) {
			{
				std::unique_lock<std::mutex> ul(m_lock);
				if (m_status == Status::Stopped)
					break;
				m_cv.wait_for(ul, std::chrono::milliseconds(1));
			}
		}
		m_cv.notify_all();
	}

	QueueState::Ptr GetQueueState() { return m_pQueueState; }

	void Run() {
		m_status = Status::Initial;
		m_thread = std::thread([=] {
			{
				std::unique_lock<std::mutex> ul(m_lock);
				m_cv.wait(ul, [=] { return m_status != Status::Initial; });
			}
			for (;;) {
				if (m_pQueueState != nullptr) {
					IExecuteable::Ptr pCmd;
					if (m_pQueue->Get(pCmd)) {
						m_pQueueState = m_pQueueState->Handle(pCmd);
						m_cv.notify_all();
					}
					else {
						std::unique_lock<std::mutex> ul(m_lock);
						m_cv.wait_for(ul, std::chrono::milliseconds(1));
					}
				}
				else {
					IExecuteable::Ptr pCmd;
					while (m_pQueue->Get(pCmd))
						;
					m_status = Status::Stopped;
					break;
				}
			}
			});
	}

	void Start() {
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_status = Status::Running;
		}
		m_cv.notify_all();
	}

	void Join() {
		m_thread.join();
	}
};
