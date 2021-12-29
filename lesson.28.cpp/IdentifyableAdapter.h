#pragma once

#include "../lesson.03.cpp/UObject.h"

#include "IIdentifyable.h"

class IdentifyableAdapter : public IIdentifyable
{
protected:
	UObject::Ptr m_pUObject;

public:
	IdentifyableAdapter(UObject::Ptr pUObject) : m_pUObject(std::move(pUObject)) {
	}

	virtual void SetIndex(size_t pos) override {
		m_pUObject->SetProperty("self.index", pos, true);
	}
	virtual size_t GetIndex() override {
		return std::any_cast<size_t>(m_pUObject->GetProperty("self.index"));
	}
};
