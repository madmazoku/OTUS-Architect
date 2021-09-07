#include <iostream>

#include "UObject.h"
#include "MoveableAdapter.h"
#include "RotateableAdapter.h"
#include "Move.h"
#include "Rotate.h"

int main()
{
	UObject::Ptr pTank = std::make_shared<UObject>();

	pTank->SetProperty("position", Vector(0, 0));
	pTank->SetProperty("velocity", Vector(1, 1));
	pTank->SetProperty("angleVelocity", 0.1);

	IExecuteable::Ptr pTankMove = std::make_shared<Move>(std::make_shared<MoveableAdapter>(pTank));
	IExecuteable::Ptr pTankRotate = std::make_shared<Rotate>(std::make_shared<RotateableAdapter>(pTank));

	pTankMove->Execute();
	pTankRotate->Execute();
}