#pragma once

#include <any>
#include "IFileMatrixSumable.h"
#include "../lesson.03.cpp/UObject.h"

// Adapter for holding sum matrix command paramteres in UObject
class FileMatrixSumableAdapter : public IFileMatrixSumable
{
protected:
	UObject::Ptr m_pUObject;

public:
	FileMatrixSumableAdapter(UObject::Ptr pUObject) : m_pUObject(pUObject) {}

	virtual std::string GetSummary() const override {
		return std::any_cast<std::string>(m_pUObject->GetProperty("summary"));
	}
	virtual void SetSummary(const std::string& summary) override {
		m_pUObject->SetProperty("summary", summary);
	}

	virtual std::vector<std::string> GetMatrixes() const override {
		return std::any_cast<std::vector<std::string>>(m_pUObject->GetProperty("matrixes"));
	}
	virtual void SetMatrixes(std::vector<std::string> matrixes) {
		m_pUObject->SetProperty("matrixes", std::move(matrixes));
	}
};
