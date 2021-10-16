#pragma once

#include <vector>
#include <algorithm>

#include "ISort.h"
#include "ICompare.h"

template<typename T>
class InsertionSort : public ISort<T>
{
protected:
	typename const ICompare<T>::Ptr m_pCompare;

public:
	InsertionSort(typename const ICompare<T>::Ptr pLessThen) : m_pCompare(pLessThen) {}

	virtual const std::string Name() const override { return std::string("Insertion-") + m_pCompare->Name(); };

	virtual void Sort(std::vector<T>& items) const override {
		if (items.size() <= 1)
			return;
		for (size_t first = 1; first < items.size(); ++first) {
			size_t index = first;
			size_t indexPrev = index - 1;
			while (m_pCompare->Compare(items[index], items[indexPrev]) == -1) {
				std::swap(items[indexPrev], items[index]);
				if (indexPrev == 0)
					break;
				index = indexPrev--;
			}
		}
	}
};
