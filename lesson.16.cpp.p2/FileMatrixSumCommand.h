#pragma once

#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <filesystem>

#include "../lesson.03.cpp/IExecuteable.h"
#include "IFileMatrixSumable.h"

class FileMatrixSumCommand : public IExecuteable
{
protected:
	IFileMatrixSumable::Ptr m_pFileMatrixSumable;

public:
	FileMatrixSumCommand(IFileMatrixSumable::Ptr pFileMatrixSumable) : m_pFileMatrixSumable(std::move(pFileMatrixSumable)) {}

	virtual void Execute() override {
		const std::string sumMatrixesCmd("lesson.16.cpp.p1.exe");

		std::stringstream ssCmd;
		ssCmd << sumMatrixesCmd << " " << m_pFileMatrixSumable->GetSummary();
		for (const std::string& matrix : m_pFileMatrixSumable->GetMatrixes())
			ssCmd << " " << matrix;

		int result = std::system(ssCmd.str().c_str());
		if (result != 0)
			throw std::runtime_error(std::string("Error cmd execution: ") + ssCmd.str());
	}
};
