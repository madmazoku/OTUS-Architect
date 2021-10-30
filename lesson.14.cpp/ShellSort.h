#pragma once

#include <vector>
#include <algorithm>

#include "ISort.h"
#include "ICompare.h"
#include "ISequence.h"

template<typename T>
class ShellSort : public ISort<T>
{
protected:
	typename const ICompare<T>::Ptr m_pCompare;
	ISequence::Ptr m_pSequence;

public:
	ShellSort(typename const ICompare<T>::Ptr pLessThen, ISequence::Ptr pShellSequence)
		: m_pCompare(std::move(pLessThen)), m_pSequence(std::move(pShellSequence))
	{}

	virtual const std::string Name() const override { return std::string("Shell-") + m_pCompare->Name() + "-" + m_pSequence->Name(); };

	virtual void Sort(std::vector<T>& items) const override {
		if (items.size() <= 1)
			return;
		size_t gap = m_pSequence->Initialize(items.size());
		for (;;) {
			for (size_t first = gap; first < items.size(); ++first)
			{
				size_t index = first;
				size_t indexPrev = index - gap;
				while (m_pCompare->Compare(items[index], items[indexPrev]) == -1) {
					std::swap(items[indexPrev], items[index]);
					if (indexPrev < gap)
						break;
					index = indexPrev;
					indexPrev -= gap;
				}
			}
			gap = m_pSequence->Decrease(gap);
			if (gap == 0)
				break;
		}
	}
};
