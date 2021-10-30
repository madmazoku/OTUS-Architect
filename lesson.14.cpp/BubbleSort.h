#pragma once

#include <vector>
#include <algorithm>

#include "ISort.h"
#include "ICompare.h"

template<typename T>
class BubbleSort : public ISort<T>
{
protected:
	typename const ICompare<T>::Ptr m_pCompare;

public:
	BubbleSort(typename const ICompare<T>::Ptr pLessThen) : m_pCompare(std::move(pLessThen)) {}

	virtual const std::string Name() const override { return std::string("Buble-") + m_pCompare->Name(); };

	virtual void Sort(std::vector<T>& items) const override {
		if (items.size() <= 1)
			return;
		bool swapped = true;
		size_t last = items.size() - 1;
		while (swapped && last > 0) {
			swapped = false;
			for (int index = 0; index < last; ++index)
				if (m_pCompare->Compare(items[index + 1], items[index]) == -1) {
					std::swap(items[index + 1], items[index]);
					swapped = true;
				}
			--last;
		}
	}
};
