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
		m_lockTail.lock();
		m_pTail = m_pTail->m_pNext = pNode;
		m_lockTail.unlock();
		return true;
	}

	virtual bool Get(T& item) override {
		Node* pNode;
		m_lockHead.lock();
		pNode = m_pHead;
		Node* pHeadNew = pNode->m_pNext;
		if (pHeadNew == nullptr) {
			m_lockHead.unlock();
			return false;
		}
		item = pHeadNew->m_item;
		pHeadNew->m_item = T();
		m_pHead = pHeadNew;
		m_lockHead.unlock();
		delete pNode;
		return true;
	}
};
