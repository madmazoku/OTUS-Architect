#pragma once

#include <mutex>

#include "Queue.h"

template<class T>
class LockQueue : public IQueue<T>
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

	std::mutex m_lock;

public:
	LockQueue() {
		m_pHead = m_pTail = nullptr;
	}
	~LockQueue() {
		while (m_pHead != nullptr) {
			Node* pNext = m_pHead;
			m_pHead = pNext->m_pNext;
			delete pNext;
		}
	}

	virtual bool Put(T item) override {
		Node* pNode = new Node(item);
		{
			std::lock_guard<std::mutex> lg(m_lock);
			if (m_pTail == nullptr)
				m_pHead = m_pTail = pNode;
			else
				m_pTail = m_pTail->m_pNext = pNode;
		}
		return true;
	}

	virtual bool Get(T& item) override {
		Node* pNode;
		{
			std::lock_guard<std::mutex> lg(m_lock);
			if (m_pHead == nullptr)
				return false;
			m_pHead = (pNode = m_pHead)->m_pNext;
			if (m_pTail == pNode)
				m_pTail = nullptr;
		}
		item = pNode->m_item;
		delete pNode;
		return true;
	}
};
