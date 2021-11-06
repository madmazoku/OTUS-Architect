#pragma once

#include <vector>
#include <algorithm>

#include "ISort.h"
#include "ICompare.h"

template<typename T>
class SelectionSort : public ISort<T>
{
protected:
	typename const ICompare<T>::Ptr m_pCompare;

public:
	SelectionSort(typename const ICompare<T>::Ptr pLessThen) : m_pCompare(std::move(pLessThen)) {}

	virtual const std::string Name() const override { return std::string("Selection-") + m_pCompare->Name(); };

	virtual void Sort(std::vector<T>& items) const override {
		if (items.size() <= 1)
			return;
		for (size_t first = 0; first < items.size(); ++first) {
			size_t min_index = first;
			T min_value = items[min_index];
			for (size_t index = first + 1; index < items.size(); ++index)
				if (m_pCompare->Compare(items[index], min_value) == -1) {
					min_index = index;
					min_value = items[index];
				}
			if (first != min_index)
				std::swap(items[first], items[min_index]);
		}
	}
};
