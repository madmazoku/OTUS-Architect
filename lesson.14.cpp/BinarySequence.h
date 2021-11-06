#pragma once

#include "ISequence.h"

class BinarySequence : public ISequence
{
public:
	virtual const std::string Name() const override { return "Binary"; }
	virtual size_t Initialize(size_t size) const override { return size >> 1; }
	virtual size_t Decrease(size_t gap) const override { return gap >>= 1; }
};
