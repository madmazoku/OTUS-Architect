#pragma once

#include <string>
#include <vector>
#include <memory>

class IFileMatrixSumable
{
public:
	typedef std::shared_ptr < IFileMatrixSumable> Ptr;

	virtual std::string GetSummary() const = 0;
	virtual void SetSummary(const std::string&) = 0;

	virtual std::vector<std::string> GetMatrixes() const = 0;
	virtual void SetMatrixes(std::vector<std::string>) = 0;
};
