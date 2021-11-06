#pragma once

#include "ICompare.h"

template<typename T>
class DefaultCompare : public ICompare<T>
{
public:
	virtual const std::string Name() const override { return "Default"; }
	virtual short Compare(T a, T b) const override { return a == b ? 0 : (a < b ? -1 : 1); }
};
