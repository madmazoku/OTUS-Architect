
#include <iostream>

#include "UObject.h"
#include "MoveableAdapter.h"
#include "RotateableAdapter.h"
#include "Move.h"
#include "Rotate.h"

int main()
{
	UObject::Ptr tank = std::make_shared<UObject>();

	tank->SetProperty("position", Vector(0, 0));
	tank->SetProperty("velocity", Vector(1, 1));
	tank->SetProperty("angleVelocity", 0.1);

	IExecuteable::Ptr tankMove = std::make_shared<Move>(std::make_shared<MoveableAdapter>(tank));
	IExecuteable::Ptr tankRotate = std::make_shared<Rotate>(std::make_shared<RotateableAdapter>(tank));

	tankMove->Execute();
	tankRotate->Execute();

}

