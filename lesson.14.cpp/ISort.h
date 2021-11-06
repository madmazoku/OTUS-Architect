#pragma once

#include <memory>
#include <vector>

// интерфейс алгоритма сортировки
template<typename T>
class ISort
{
public:
	typedef std::shared_ptr<ISort<T>> Ptr;

	virtual const std::string Name() const = 0;
	virtual void Sort(std::vector<T>&) const = 0;
};
