#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>

template<class T, template<class> class Q>
class QueueThread {
public:
	typedef std::shared_ptr<QueueThread> Ptr;

protected:
	Q<T> m_queue;

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
	QueueThread() : m_status(Status::Initial) {}

	void Put(T item) {
		m_queue.Put(item);
		m_cv.notify_one();
	}

	void HardStop() {
		T item;
		while (m_queue.Get(item))
			;
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_status = Status::Stopped;
		}
		m_cv.notify_one();
	}

	void SoftStop() {
		m_status = Status::Stopped;
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_status = Status::Stopped;
		}
		m_cv.notify_one();
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
				if (m_queue.Get(item))
					lambda(item);
				else {
					std::unique_lock<std::mutex> ul(m_lock);
					if (m_status != Status::Running)
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
		m_cv.notify_one();
	}

	void Join() {
		m_thread.join();
	}
};