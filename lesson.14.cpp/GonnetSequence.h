#pragma once

#include "ISequence.h"

class GonnetSequence : public ISequence
{
public:
	virtual const std::string Name() const override { return "Gonnet"; }
	virtual size_t Initialize(size_t size) const override { return 5 * size / 11; }
	virtual size_t Decrease(size_t gap) const override {
		if (gap > 2)
			return 5 * gap / 11;
		else if (gap > 1)
			return 1;
		else
			return 0;
	}
};
