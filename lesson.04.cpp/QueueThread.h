#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>

#include "Queue.h"

template<class T>
class QueueThread {
public:
	typedef std::shared_ptr<QueueThread> Ptr;

protected:
	typedef typename IQueue<T>::Ptr IQueuePtr;
	IQueuePtr m_pQueue;

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
	QueueThread(IQueuePtr pQueue) : m_pQueue(pQueue), m_status(Status::Initial) {}

	void Put(T item) {
		while (m_status != Status::Stopped && !m_pQueue->Put(item)) {
			{
				std::unique_lock<std::mutex> ul(m_lock);
				if (m_status == Status::Stopped)
					break;
				m_cv.wait(ul);
			}
		}
		m_cv.notify_all();
	}

	void HardStop() {
		T item;
		while (m_pQueue->Get(item))
			;
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_status = Status::Stopped;
		}
		m_cv.notify_all();
	}

	void SoftStop() {
		m_status = Status::Stopped;
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_status = Status::Stopped;
		}
		m_cv.notify_all();
	}

	void Run(std::function<void(T)> lambda) {
		m_status = Status::Initial;
		m_thread = std::thread([=] {
			{
				std::unique_lock<std::mutex> ul(m_lock);
				m_cv.wait(ul, [=] { return m_status != Status::Initial; });
			}
			for (;;) {
				T item;
				if (m_pQueue->Get(item)) {
					lambda(item);
					m_cv.notify_all();
				}
				else {
					std::unique_lock<std::mutex> ul(m_lock);
					if (m_status == Status::Stopped)
						break;
					m_cv.wait(ul);
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