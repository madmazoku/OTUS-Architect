#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

class Waiter {
protected:
	std::mutex m_lock;
	std::condition_variable m_cv;
	bool m_pulse = false;

public:
	Waiter() : m_pulse(false) {}

	void Wait() {
		{
			std::unique_lock<std::mutex> ul(m_lock);
			m_cv.wait(ul, [&] {return m_pulse; });
			m_pulse = false;
		}
		m_cv.notify_one();
	}

	void Pulse() {
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_pulse = true;
		}
		m_cv.notify_one();
	}
};