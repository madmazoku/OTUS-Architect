#pragma once

#include <string>

#include "ICompare.h"
#include "DefaultCompare.h"

class AsNumberCompare : public ICompare<std::string>
{
protected:
	ICompare<long long>::Ptr m_pCompare;

public:
	AsNumberCompare(ICompare<long long>::Ptr pCompare = std::make_shared<DefaultCompare<long long>>()) : m_pCompare(std::move(pCompare)) {}

	virtual const std::string Name() const override { return std::string("AsNumber") + m_pCompare->Name(); }
	virtual short Compare(std::string a, std::string b) const override { return m_pCompare->Compare(std::stoll(a), std::stoll(b)); }
};
