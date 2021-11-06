#pragma once

#include "ICompare.h"

template<typename T>
class InverseCompare : public ICompare<T>
{
protected:
	typename const ICompare<T>::Ptr m_pCompare;

public:
	InverseCompare(typename const ICompare<T>::Ptr pLessThen) : m_pCompare(pLessThen) {}
	virtual const std::string Name() const override { return std::string("Inverse") + m_pCompare->Name(); }
	virtual short Compare(T a, T b) const override { return -m_pCompare->Compare(a, b); }
};
