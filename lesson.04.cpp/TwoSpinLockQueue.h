#pragma once

#include <thread>
#include <atomic>

class SpinLock
{
protected:
	std::atomic_flag m_locked;

public:
	void lock() {
		while (true) {
			if (!m_locked.test_and_set(std::memory_order_acquire))
				break;
			std::this_thread::sleep_for(std::chrono::nanoseconds(250));
		}
	}
	void unlock() {
		m_locked.clear(std::memory_order_release);
	}
};

class SpinLockGuard {
protected:
	SpinLock& m_lock;

public:
	SpinLockGuard(SpinLock& lock) : m_lock(lock) {
		m_lock.lock();
	}
	~SpinLockGuard() {
		m_lock.unlock();
	}
};

template<class T>
class TwoSpinLockQueue : public IQueue<T>
{
protected:
	class Node {
	public:
		T m_item;
		Node* m_pNext;

		Node() : m_pNext(nullptr) {}
		Node(T item) : m_item(item), m_pNext(nullptr) {}
	};

	Node* m_pHead;
	Node* m_pTail;

	SpinLock m_lockHead;
	SpinLock m_lockTail;

public:
	TwoSpinLockQueue() {
		m_pHead = m_pTail = new Node();
	}
	~TwoSpinLockQueue() {
		while (m_pHead != nullptr) {
			Node* pNext = m_pHead;
			m_pHead = pNext->m_pNext;
			delete pNext;
		}
	}

	virtual bool Put(T item) override {
		Node* pNode = new Node(item);
		{
			SpinLockGuard lg(m_lockTail);
			m_pTail = m_pTail->m_pNext = pNode;
		}
		return true;
	}

	virtual bool Get(T& item) override {
		Node* pNode;
		{
			SpinLockGuard lg(m_lockHead);
			pNode = m_pHead;
			Node* pHeadNew = pNode->m_pNext;
			if (pHeadNew == nullptr)
				return false;
			item = pHeadNew->m_item;
			pHeadNew->m_item = T();
			m_pHead = pHeadNew;
		}
		delete pNode;
		return true;
	}
};
