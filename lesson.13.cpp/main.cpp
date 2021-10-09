#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>

#include "../lesson.03.cpp/UObject.h"
#include "IFuelable.h"
#include "FuelableAdapter.h"
#include "../lesson.03.cpp/IExecuteable.h"
#include "CheckFuelCommand.h"
#include "CommandException.h"
#include "BurnFuelCommand.h"
#include "MacroCommand.h"

#include "DirectionRotateableAdapter.h"
#include "DirectionRotate.h"
#include "ChangeVelocityCommand.h"

#include "../lesson.02.cpp/Vector.h"
#include "../lesson.03.cpp/IMoveable.h"
#include "../lesson.03.cpp/MoveableAdapter.h"
#include "../lesson.03.cpp/Move.h"

#include <list>

int main()
{
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IMoveable::Ptr moveable = std::make_shared<MoveableAdapter>(obj);
	IDirectionRotateable::Ptr directionRotateable = std::make_shared<DirectionRotateableAdapter>(obj);

	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);
	IExecuteable::Ptr move = std::make_shared<Move>(moveable);
	IExecuteable::Ptr directionRotate = std::make_shared<DirectionRotate>(directionRotateable);
	IExecuteable::Ptr changeVelocity = std::make_shared<ChangeVelocityCommand>(moveable, directionRotateable);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	MacroCommand::CommandList commands;
	commands.push_back(checkFuel);
	commands.push_back(directionRotate);
	commands.push_back(changeVelocity);
	commands.push_back(move);
	commands.push_back(burnFuel);

	IExecuteable::Ptr macro = std::make_shared<MacroCommand>(commands);

	fuelable->SetFuelLevel(100);
	fuelable->SetFuelCost(75);
	moveable->SetPosition({ 2, 3 });
	moveable->SetVelocity({ 3, 4 });
	directionRotateable->SetDirection({ 0, 1 });
	directionRotateable->SetAngleVelocity(M_PI_2);

	std::cout << "Start" << std::endl;
	std::cout << "Fuel: " << fuelable->GetFuelLevel() << " / " << fuelable->GetFuelCost() << std::endl;
	std::cout << "Move: " << moveable->GetPosition() << " / " << moveable->GetVelocity() << std::endl;
	std::cout << "DirectionRotate: " << directionRotateable->GetDirection() << " / " << directionRotateable->GetAngleVelocity() << std::endl;

	std::cout << "1st Execute" << std::endl;
	macro->Execute();

	std::cout << "Fuel: " << fuelable->GetFuelLevel() << " / " << fuelable->GetFuelCost() << std::endl;
	std::cout << "Move: " << moveable->GetPosition() << " / " << moveable->GetVelocity() << std::endl;
	std::cout << "DirectionRotate: " << directionRotateable->GetDirection() << " / " << directionRotateable->GetAngleVelocity() << std::endl;

	std::cout << "2nd Execute" << std::endl;
	try {
		macro->Execute();
	}
	catch (CommandException&) {
		std::cout << "Exception catched" << std::endl;
	}

	std::cout << "Fuel: " << fuelable->GetFuelLevel() << " / " << fuelable->GetFuelCost() << std::endl;
	std::cout << "Move: " << moveable->GetPosition() << " / " << moveable->GetVelocity() << std::endl;
	std::cout << "DirectionRotate: " << directionRotateable->GetDirection() << " / " << directionRotateable->GetAngleVelocity() << std::endl;
}