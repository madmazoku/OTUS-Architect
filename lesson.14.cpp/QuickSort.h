#pragma once

#include <vector>
#include <algorithm>
#include <stack>
#include <tuple>

#include "ISort.h"
#include "ICompare.h"

template<typename T>
class QuickSort : public ISort<T>
{
protected:
	typename const ICompare<T>::Ptr m_pCompare;

	size_t PartItems(std::vector<T>& items, size_t leftIndex, size_t rightIndex) const {
		size_t pivotIndex = leftIndex + ((rightIndex - leftIndex + 1) >> 1);
		std::swap(items[leftIndex], items[pivotIndex]);

		size_t left = leftIndex;
		size_t right = rightIndex + 1;
		for (;;) {
			while (m_pCompare->Compare(items[++left], items[leftIndex]) == -1)
				if (left == rightIndex)
					break;
			while (m_pCompare->Compare(items[leftIndex], items[--right]) == -1)
				if (right == leftIndex)
					break;
			if (left >= right)
				break;
			std::swap(items[left], items[right]);
		};

		std::swap(items[leftIndex], items[right]);
		return right;
	}

public:
	QuickSort(typename const ICompare<T>::Ptr pLessThen) : m_pCompare(pLessThen) {}

	virtual const std::string Name() const override { return std::string("Quick-") + m_pCompare->Name(); };

	virtual void Sort(std::vector<T>& items) const override {
		if (items.size() <= 1)
			return;
		std::stack<std::tuple<size_t, size_t>> parts;
		parts.push(std::make_tuple<size_t, size_t>(0, items.size()));
		while (!parts.empty()) {
			size_t start, end;
			std::tie(start, end) = parts.top();
			parts.pop();

			size_t parted = PartItems(items, start, end - 1);
			if (parted - start < end - parted - 1) {
				if (parted + 2 < end)
					parts.push(std::make_pair(parted + 1, end));
				if (start + 1 < parted)
					parts.push(std::make_pair(start, parted));
			}
			else {
				if (start + 1 < parted)
					parts.push(std::make_pair(start, parted));
				if (parted + 2 < end)
					parts.push(std::make_pair(parted + 1, end));
			}
		}
	}
};
