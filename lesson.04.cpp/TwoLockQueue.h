#pragma once

#include <mutex>

template<class T>
class TwoLockQueue
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

	std::mutex m_lockHead;
	std::mutex m_lockTail;

public:
	TwoLockQueue() {
		m_pHead = m_pTail = new Node();
	}
	~TwoLockQueue() {
		while (m_pHead != nullptr) {
			Node* pNext = m_pHead;
			m_pHead = pNext->m_pNext;
			delete pNext;
		}
	}

	void Put(T item) {
		Node* pNode = new Node(item);
		{
			std::lock_guard<std::mutex> lg(m_lockTail);
			m_pTail = m_pTail->m_pNext = pNode;
		}
	}

	bool Get(T& item) {
		Node* pNode;
		{
			std::lock_guard<std::mutex> lg(m_lockHead);
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
