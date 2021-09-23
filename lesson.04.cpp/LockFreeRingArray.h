#pragma once

#include <atomic>
#include <array>
#include <memory>

#include "IQueue.h"

template<class T, unsigned int BITS = 8>
class LockFreeRingArray : public IQueue<T> {
protected:
	static const unsigned long g_SIZE = 1 << BITS;
	static const unsigned long g_MASK = g_SIZE - 1;

	typedef std::pair<T, bool> TFlagged;

	std::array<TFlagged, g_SIZE> m_buffer;
	std::atomic<unsigned long> m_read;
	std::atomic<unsigned long> m_write;

public:
	LockFreeRingArray() : m_read(0), m_write(0) {
		for (TFlagged& item : m_buffer)
			item.second = false;
	}

	virtual bool Put(T item) override {
		unsigned long write;
		do {
			write = m_write;
			if (write - m_read == g_SIZE)
				return false;
		} while (!m_buffer[write & g_MASK].second && !m_write.compare_exchange_weak(write, write + 1));

		m_buffer[write & g_MASK].first = item;
		m_buffer[write & g_MASK].second = true;

		return true;
	}

	virtual bool Get(T& item) override {
		unsigned long read;
		do {
			read = m_read;
			if (read == m_write)
				return false;
		} while (m_buffer[read & g_MASK].second && !m_read.compare_exchange_weak(read, read + 1));

		item = m_buffer[read & g_MASK].first;
		m_buffer[read & g_MASK].first = T();
		m_buffer[read & g_MASK].second = false;

		return true;
	}
};
