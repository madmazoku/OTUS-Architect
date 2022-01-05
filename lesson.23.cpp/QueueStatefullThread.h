#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iostream>
#include <stdexcept>

#include "QueueState.h"

#include "../lesson.04.cpp/IQueue.h"
#include "../lesson.03.cpp/IExecuteable.h"

typedef IQueue<IExecuteable::Ptr> IExecuteableQueue;
// ќчередь выполнени€ комманд
// ѕозвод€ет использовать состо€ние дл€ задани€ последовательности
// операций над каждой командой в очереди
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
	Status m_status = Status::Initial;

	std::mutex m_lock;
	std::condition_variable m_cv;
	std::thread m_thread;

public:
	QueueStatefullThread(IExecuteableQueue::Ptr pQueue, QueueState::Ptr pInitialQueueState) : m_pQueue(std::move(pQueue)), m_pQueueState(std::move(pInitialQueueState)) {}
	virtual ~QueueStatefullThread() {
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_pQueueState = nullptr;
		}
		m_cv.notify_all();
		if (m_thread.joinable())
			m_thread.join();
	}

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
		{
			std::lock_guard<std::mutex> lg(m_lock);
			if (m_status != Status::Initial)
				throw std::runtime_error("Sequental attempt to create queue thread");
		}
		m_thread = std::thread([=] {
			{
				std::unique_lock<std::mutex> ul(m_lock);
				m_cv.wait(ul, [=] { return m_status != Status::Initial; });
			}
			for (;;) {
				QueueState::Ptr pQueueState;
				{
					std::lock_guard<std::mutex> lg(m_lock);
					pQueueState = m_pQueueState;
				}
				if (pQueueState != nullptr) {
					IExecuteable::Ptr pCmd;
					if (m_pQueue->Get(pCmd)) {
						pQueueState = pQueueState->Handle(pCmd);
						{
							std::lock_guard<std::mutex> lg(m_lock);
							m_pQueueState = pQueueState;
						}
						m_cv.notify_all();
					}
					else {
						std::unique_lock<std::mutex> ul(m_lock);
						m_cv.wait_for(ul, std::chrono::milliseconds(1));
					}
				}
				else {
					m_status = Status::Stopped;
					break;
				}
			}
			m_cv.notify_all();
			});
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_status = Status::Running;
		}
		m_cv.notify_all();
	}

	void Join() {
		if (m_thread.joinable())
			m_thread.join();
	}
};
