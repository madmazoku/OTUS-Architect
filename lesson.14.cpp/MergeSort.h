#pragma once

#include <vector>
#include <algorithm>

#include "ISort.h"
#include "ICompare.h"

template<typename T>
class MergeSort : public ISort<T>
{
protected:
	typename const ICompare<T>::Ptr m_pCompare;

	void SortAux(std::vector<T>& items, std::vector<T>& aux, size_t start, size_t mid, size_t end) const {
		std::copy(items.begin() + start, items.begin() + mid, aux.begin() + start);
		size_t left = start;
		size_t right = mid;
		size_t index = start;
		while (left < mid && right < end)
			if (m_pCompare->Compare(aux[left], items[right]) == -1)
				items[index++] = aux[left++];
			else
				items[index++] = items[right++];
		std::copy(aux.begin() + left, aux.begin() + mid, items.begin() + index);
	}

public:
	MergeSort(typename const ICompare<T>::Ptr pLessThen) : m_pCompare(std::move(pLessThen)) {}

	virtual const std::string Name() const override { return std::string("Merge-") + m_pCompare->Name(); };

	virtual void Sort(std::vector<T>& items) const override {
		if (items.size() <= 1)
			return;
		std::vector<T> aux(items.size());
		for (size_t size = 1; size < items.size(); size <<= 1)
			for (size_t start = 0; start < items.size(); start += (size << 1)) {
				size_t mid = start + size;
				size_t end = mid + size;
				if (mid < items.size()) {
					if (end > items.size())
						end = items.size();
					SortAux(items, aux, items.size() - end, items.size() - mid, items.size() - start);
				}
			}
	}
};
