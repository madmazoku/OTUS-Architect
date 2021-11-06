#pragma once

#include "ISequence.h"

class KnuthSequence : public ISequence
{
public:
	virtual const std::string Name() const override { return "Knuth"; }
	virtual size_t Initialize(size_t size) const override {
		size_t sizeDiv3 = size / 3;
		size_t gap = 1;
		while (gap < sizeDiv3)
			gap = 3 * gap + 1;
		return gap;
	}
	virtual size_t Decrease(size_t gap) const override {
		if (gap > 3)
			return gap - gap / 3;
		else if (gap > 1)
			return 1;
		else
			return 0;
	}
};
