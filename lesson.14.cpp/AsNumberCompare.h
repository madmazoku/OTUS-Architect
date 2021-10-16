#pragma once

#include <string>

#include "ICompare.h"
#include "DefaultCompare.h"

class AsNumberCompare : public ICompare<std::string>
{
protected:
	ICompare<long long>::Ptr m_pCompare;

public:
	AsNumberCompare() : m_pCompare(std::make_shared<DefaultCompare<long long>>()) {}

	virtual const std::string Name() const override { return "AsNumber"; }
	virtual short Compare(std::string a, std::string b) const override { return m_pCompare->Compare(std::stoll(a), std::stoll(b)); }
};
