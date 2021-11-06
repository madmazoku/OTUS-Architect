#pragma once

#include <memory>

// интерфейс последовательности промежутков для алгоритма сортировки Шелла
class ISequence
{
public:
	typedef std::shared_ptr<ISequence> Ptr;

	virtual const std::string Name() const = 0;
	virtual size_t Initialize(size_t) const = 0;
	virtual size_t Decrease(size_t) const = 0;
};
