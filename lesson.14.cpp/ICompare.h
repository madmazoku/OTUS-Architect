#pragma once

#include <memory>

template<typename T>
class ICompare
{
public:
	typedef std::shared_ptr<ICompare<T>> Ptr;

	virtual const std::string Name() const = 0;
	virtual short Compare(T a, T b) const = 0;
};
