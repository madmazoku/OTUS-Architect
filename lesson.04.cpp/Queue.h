#pragma once

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <stdexcept>
#include <limits>
#include <functional>

template<class T>
class ThreadQueue
{
public:
	typedef std::shared_ptr<ThreadQueue<T>> Ptr;

	class QueueException : public std::exception {};
	class QueueStopException : public QueueException {};

protected:
	std::queue<T> m_queue;
	unsigned long m_limit;

	std::mutex m_printLock;

	std::mutex m_lock;
	std::condition_variable m_condition;
	std::shared_ptr<std::thread> m_pThread;

	enum class Status
	{
		Initial,
		Ready,
		Running,
		Stop,
	};
	Status m_status;

public:
	ThreadQueue(unsigned long limit = std::numeric_limits<unsigned long>::max()) : m_limit(limit), m_status(Status::Initial) {}

	void Put(T item) {
		std::unique_lock<std::mutex> ul(m_lock);

		m_condition.wait(ul, [=]() { return m_status != Status::Initial && (m_status != Status::Running || m_queue.size() < m_limit); });
		if (m_status != Status::Running) {
			ul.unlock();
			m_condition.notify_all();
			throw QueueStopException();
		}

		m_queue.push(item);

		ul.unlock();
		m_condition.notify_all();
	}

	T Get() {
		std::unique_lock<std::mutex> ul(m_lock);

		m_condition.wait(ul, [=]() { return m_status != Status::Initial && (m_status != Status::Running || !m_queue.empty()); });
		if (m_status != Status::Running && m_queue.empty()) {
			ul.unlock();
			m_condition.notify_all();
			throw QueueStopException();
		}

		T item = m_queue.front();
		m_queue.pop();

		ul.unlock();
		m_condition.notify_all();

		return item;
	}

	void SoftStop() {
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_status = Status::Stop;
		}
		m_condition.notify_all();
		m_pThread->join();

		m_status = Status::Initial;
		m_pThread = nullptr;
	}

	void HardStop() {
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_status = Status::Stop;

			while (!m_queue.empty())
				m_queue.pop();
		}
		m_condition.notify_all();
		m_pThread->join();

		m_status = Status::Initial;
		m_pThread = nullptr;
	}

	void Start() {
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_status = Status::Running;
		}
		m_condition.notify_all();
	}

#pragma warning( push )
#pragma warning(disable: 4101)

	void Run(std::function<void(T)> lambda, bool initialSuspend) {
		std::lock_guard<std::mutex> lg(m_lock);
		m_status = initialSuspend ? Status::Initial : Status::Running;

		m_pThread = std::make_shared<std::thread>([=] {
			{
				std::unique_lock<std::mutex> ul(m_lock);
				m_condition.wait(ul, [=] {return m_status != Status::Initial; });
			}
			try {
				while (true) {
					T item = Get();
					lambda(item);
				}
			}
			catch (QueueStopException const& ex) {
				;
			}
			});
	}

#pragma warning( pop )
};
