#pragma once

#include <vector>
#include <algorithm>

#include "ISort.h"

template<typename T>
class HeapSort : public ISort<T>
{
protected:
	typename const ICompare<T>::Ptr m_pCompare;

	void Heapify(std::vector<T>& items) const {
		size_t root = items.size() >> 1;
		do {
			--root;
			SinkDown(items, root, items.size());
		} while (root > 0);
	}

	void SinkDown(std::vector<T>& items, size_t root, size_t size) const {
		for (;;) {
			size_t maxIndex = root;
			size_t leftIndex = (root << 1) + 1;
			size_t rightIndex = leftIndex + 1;
			if (leftIndex < size) {
				if (m_pCompare->Compare(items[maxIndex], items[leftIndex]) == -1)
					maxIndex = leftIndex;
				if (rightIndex < size&& m_pCompare->Compare(items[maxIndex], items[rightIndex]) == -1)
					maxIndex = rightIndex;
			}
			if (maxIndex == root)
				break;
			std::swap(items[root], items[maxIndex]);
			root = maxIndex;
		}
	}

public:
	HeapSort(typename const ICompare<T>::Ptr pLessThen) : m_pCompare(pLessThen) {}

	virtual const std::string Name() const override { return std::string("Heap-") + m_pCompare->Name(); };

	virtual void Sort(std::vector<T>& items) const override {
		if (items.size() <= 1)
			return;

		Heapify(items);
		for (size_t size = items.size(); size > 0; --size) {
			SinkDown(items, 0, size);
			std::swap(items[0], items[size - 1]);
		}
	}
};
