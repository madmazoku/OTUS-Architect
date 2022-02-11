#pragma once

#include "IGetPlayer.h"

#include "OwnableAdapter.h"
#include "IoC.h"

class GetPlayer : public IGetPlayer
{
public:
	GetPlayer() {}

	virtual size_t Get(UObject::Ptr pUObject) override {
		return IoC->Resolve<size_t>("Tank.GetPlayer", { pUObject });
	}
};
