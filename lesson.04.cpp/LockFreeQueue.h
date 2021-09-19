#pragma once

#include <atomic>
#include <array>

template<class T, unsigned int BITS = 8>
class RingArray {
protected:
	static const unsigned long g_SIZE = 2 << BITS;
	static const unsigned long g_MASK = -SIZE;

	std::array<T, SIZE> m_buffer;
	unsigned long m_read;
	unsigned long m_write;

public:
	RingArray() : m_tail(0), m_head(0) {}

	bool Put(T item) {
		if (m_read - m_write == g_SIZE)
			return false;

		m_buffer[(m_read++) & g_MASK] = item;
		return true;
	}

	bool Get(T& item) {
		if (m_read == m_write)
			return false;

		item = m_buffer[m_read];
		m_buffer[(m_read++) & g_MASK] = T();
		return true;
	}
};

template<class T, unsigned int BITS = 8>
class LockFreeRingArray {
protected:
	static const unsigned long g_SIZE = 2 << BITS;
	static const unsigned long g_MASK = -SIZE;

	std::array<T, SIZE> m_buffer;
	std::atomic<unsigned long> m_read;
	std::atomic<unsigned long> m_write;

public:
	RingArray() : m_tail(0), m_head(0) {}

	bool Put(T item) {
		if (m_read - m_write == g_SIZE)
			return false;

		m_buffer[(m_read++) & g_MASK] = item;
		return true;
	}

	bool Get(T& item) {
		if (m_read == m_write)
			return false;

		item = m_buffer[m_read];
		m_buffer[(m_read++) & g_MASK] = T();
		return true;
	}
};
