#pragma once

#include "../lesson.03.cpp/UObject.h"

#include "IOwnable.h"

class OwnableAdapter : public IOwnable
{
protected:
	UObject::Ptr m_pUObject;

public:
	OwnableAdapter(UObject::Ptr pUObject) : m_pUObject(std::move(pUObject)) {
	}

	virtual void SetOwnerIndex(size_t pos) override {
		m_pUObject->SetProperty("self.owner.index", pos);
	}
	virtual size_t GetOwnerIndex() override {
		return std::any_cast<size_t>(m_pUObject->GetProperty("self.owner.index"));
	}
};
