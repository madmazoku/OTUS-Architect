#include "pch.h"

#include <cmath>
#include <stdexcept>

#include "../lesson.03.cpp/UObject.h"
#include "../lesson.13.cpp/IFuelable.h"
#include "../lesson.13.cpp/FuelableAdapter.h"
#include "../lesson.03.cpp/IExecuteable.h"
#include "../lesson.13.cpp/CheckFuelCommand.h"
#include "../lesson.13.cpp/CommandException.h"
#include "../lesson.13.cpp/BurnFuelCommand.h"
#include "../lesson.13.cpp/MacroCommand.h"
#include "../lesson.13.cpp/DirectionRotateableAdapter.h"
#include "../lesson.13.cpp/DirectionRotate.h"
#include "../lesson.13.cpp/ChangeVelocityCommand.h"

#include "../lesson.02.cpp/Vector.h"
#include "../lesson.03.cpp/IMoveable.h"
#include "../lesson.03.cpp/MoveableAdapter.h"
#include "../lesson.03.cpp/Move.h"

#include "../lesson.03.cpp/IRotateable.h"
#include "../lesson.03.cpp/RotateableAdapter.h"
#include "../lesson.03.cpp/Rotate.h"

TEST(CheckFuelCommand, Success) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);

	fuelable->SetFuelLevel(100);
	fuelable->SetFuelCost(75);

	checkFuel->Execute();

	EXPECT_EQ(fuelable->GetFuelLevel(), 100);
	EXPECT_EQ(fuelable->GetFuelCost(), 75);
}

TEST(CheckFuelCommand, NoFuelLevel) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);

	fuelable->SetFuelCost(75);

	EXPECT_THROW({ checkFuel->Execute(); }, std::invalid_argument);
}

TEST(CheckFuelCommand, NoFuelCost) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);

	fuelable->SetFuelCost(75);

	EXPECT_THROW({ checkFuel->Execute(); }, std::invalid_argument);
}

TEST(CheckFuelCommand, NotEnoughFuelLevel) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);

	fuelable->SetFuelLevel(25);
	fuelable->SetFuelCost(75);

	EXPECT_THROW({ checkFuel->Execute(); }, CommandException);
}

TEST(BurnFuelCommand, Success) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelLevel(100);
	fuelable->SetFuelCost(75);

	burnFuel->Execute();

	EXPECT_EQ(fuelable->GetFuelLevel(), 25);
	EXPECT_EQ(fuelable->GetFuelCost(), 75);
}

TEST(BurnFuelCommand, NoFuelLevel) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelCost(75);

	EXPECT_THROW({ burnFuel->Execute(); }, std::invalid_argument);
}

TEST(BurnFuelCommand, NoFuelCost) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelCost(75);

	EXPECT_THROW({ burnFuel->Execute(); }, std::invalid_argument);
}

TEST(BurnFuelCommand, NotEnoughFuelLevel) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelLevel(25);
	fuelable->SetFuelCost(75);

	EXPECT_THROW({ burnFuel->Execute(); }, CommandException);
}

TEST(DirectionRotateable, ZeroDirectionLength) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IDirectionRotateable::Ptr directionRotateable = std::make_shared<DirectionRotateableAdapter>(obj);

	EXPECT_THROW({ directionRotateable->SetDirection({ 0, 0 }); }, std::invalid_argument);
}

TEST(DirectionRotate, Success) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IDirectionRotateable::Ptr directionRotateable = std::make_shared<DirectionRotateableAdapter>(obj);
	IExecuteable::Ptr directionRotate = std::make_shared<DirectionRotate>(directionRotateable);

	directionRotateable->SetDirection({ 2, 0 });
	directionRotateable->SetAngleVelocity(M_PI_2);

	directionRotate->Execute();

	Vector direction = directionRotateable->GetDirection();

	EXPECT_NEAR(direction.m_x, 0, 1e-12);
	EXPECT_DOUBLE_EQ(direction.m_y, -1);
	EXPECT_DOUBLE_EQ(directionRotateable->GetAngleVelocity(), M_PI_2);
}

TEST(DirectionRotate, NoDirection) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IDirectionRotateable::Ptr directionRotateable = std::make_shared<DirectionRotateableAdapter>(obj);
	IExecuteable::Ptr directionRotate = std::make_shared<DirectionRotate>(directionRotateable);

	directionRotateable->SetAngleVelocity(M_PI_2);

	EXPECT_THROW({ directionRotate->Execute(); }, std::invalid_argument);
}

TEST(DirectionRotate, NoAngleVelocity) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IDirectionRotateable::Ptr directionRotateable = std::make_shared<DirectionRotateableAdapter>(obj);
	IExecuteable::Ptr directionRotate = std::make_shared<DirectionRotate>(directionRotateable);

	directionRotateable->SetDirection({ 2, 0 });

	EXPECT_THROW({ directionRotate->Execute(); }, std::invalid_argument);
}

TEST(ChangeVelocityCommand, Success) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IMoveable::Ptr moveable = std::make_shared<MoveableAdapter>(obj);
	IDirectionRotateable::Ptr directionRotateable = std::make_shared<DirectionRotateableAdapter>(obj);
	IExecuteable::Ptr changeVelocity = std::make_shared<ChangeVelocityCommand>(moveable, directionRotateable);

	moveable->SetVelocity({ 3,4 });
	directionRotateable->SetDirection({ 2, 0 });

	changeVelocity->Execute();

	Vector velocity = moveable->GetVelocity();
	Vector direction = directionRotateable->GetDirection();

	EXPECT_DOUBLE_EQ(velocity.m_x, 5);
	EXPECT_DOUBLE_EQ(velocity.m_y, 0);
	EXPECT_DOUBLE_EQ(direction.m_x, 1);
	EXPECT_DOUBLE_EQ(direction.m_y, 0);
}

TEST(ChangeVelocityCommand, NoVelocity) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IMoveable::Ptr moveable = std::make_shared<MoveableAdapter>(obj);
	IDirectionRotateable::Ptr directionRotateable = std::make_shared<DirectionRotateableAdapter>(obj);
	IExecuteable::Ptr changeVelocity = std::make_shared<ChangeVelocityCommand>(moveable, directionRotateable);

	directionRotateable->SetDirection({ 2, 0 });

	EXPECT_THROW({ changeVelocity->Execute(); }, std::invalid_argument);
}

TEST(ChangeVelocityCommand, NoDirection) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IMoveable::Ptr moveable = std::make_shared<MoveableAdapter>(obj);
	IDirectionRotateable::Ptr directionRotateable = std::make_shared<DirectionRotateableAdapter>(obj);
	IExecuteable::Ptr changeVelocity = std::make_shared<ChangeVelocityCommand>(moveable, directionRotateable);

	moveable->SetVelocity({ 3,4 });

	EXPECT_THROW({ changeVelocity->Execute(); }, std::invalid_argument);
}

TEST(MacroCommand, Success) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelLevel(100);
	fuelable->SetFuelCost(75);

	MacroCommand::CommandList commands{ checkFuel, burnFuel };
	IExecuteable::Ptr macro = std::make_shared<MacroCommand>(commands);

	macro->Execute();

	EXPECT_EQ(fuelable->GetFuelLevel(), 25);
	EXPECT_EQ(fuelable->GetFuelCost(), 75);
}

TEST(MacroCommand, NotEnoughFuelLevel) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelLevel(25);
	fuelable->SetFuelCost(75);

	MacroCommand::CommandList commands{ checkFuel, burnFuel };
	IExecuteable::Ptr macro = std::make_shared<MacroCommand>(commands);

	EXPECT_THROW({ macro->Execute(); }, CommandException);
}

TEST(MacroCommand, MoveSuccess) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IMoveable::Ptr moveable = std::make_shared<MoveableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);
	IExecuteable::Ptr move = std::make_shared<Move>(moveable);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelLevel(100);
	fuelable->SetFuelCost(75);
	moveable->SetPosition({ 2, 3 });
	moveable->SetVelocity({ 1, 2 });

	MacroCommand::CommandList commands{ checkFuel, move, burnFuel };
	IExecuteable::Ptr macro = std::make_shared<MacroCommand>(commands);

	macro->Execute();

	Vector position = moveable->GetPosition();
	Vector velocity = moveable->GetVelocity();

	EXPECT_EQ(fuelable->GetFuelLevel(), 25);
	EXPECT_EQ(fuelable->GetFuelCost(), 75);

	EXPECT_DOUBLE_EQ(position.m_x, 3);
	EXPECT_DOUBLE_EQ(position.m_y, 5);
	EXPECT_DOUBLE_EQ(velocity.m_x, 1);
	EXPECT_DOUBLE_EQ(velocity.m_y, 2);
}

TEST(MacroCommand, MoveNotEnoughFuelLevel) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IMoveable::Ptr moveable = std::make_shared<MoveableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);
	IExecuteable::Ptr move = std::make_shared<Move>(moveable);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelLevel(25);
	fuelable->SetFuelCost(75);
	moveable->SetPosition({ 2, 3 });
	moveable->SetVelocity({ 1, 2 });

	MacroCommand::CommandList commands{ checkFuel, move, burnFuel };
	IExecuteable::Ptr macro = std::make_shared<MacroCommand>(commands);

	EXPECT_THROW({ macro->Execute(); }, CommandException);
}

TEST(MacroCommand, RotateSuccess) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IRotateable::Ptr rotateable = std::make_shared<RotateableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);
	IExecuteable::Ptr rotate = std::make_shared<Rotate>(rotateable);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelLevel(100);
	fuelable->SetFuelCost(75);
	rotateable->SetVelocity({ 1, 2 });
	rotateable->SetAngleVelocity(M_PI_2); // 90°

	MacroCommand::CommandList commands{ checkFuel, rotate, burnFuel };
	IExecuteable::Ptr macro = std::make_shared<MacroCommand>(commands);

	macro->Execute();

	Vector velocity = rotateable->GetVelocity();

	EXPECT_EQ(fuelable->GetFuelLevel(), 25);
	EXPECT_EQ(fuelable->GetFuelCost(), 75);

	EXPECT_DOUBLE_EQ(velocity.m_x, 2);
	EXPECT_DOUBLE_EQ(velocity.m_y, -1);
	EXPECT_DOUBLE_EQ(rotateable->GetAngleVelocity(), M_PI_2);
}

TEST(MacroCommand, RotateStationary) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IRotateable::Ptr rotateable = std::make_shared<RotateableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);
	IExecuteable::Ptr rotate = std::make_shared<Rotate>(rotateable);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelLevel(100);
	fuelable->SetFuelCost(75);
	rotateable->SetVelocity({ 0, 0 });
	rotateable->SetAngleVelocity(M_PI_2); // 90°

	MacroCommand::CommandList commands{ checkFuel, rotate, burnFuel };
	IExecuteable::Ptr macro = std::make_shared<MacroCommand>(commands);

	macro->Execute();

	Vector velocity = rotateable->GetVelocity();

	EXPECT_EQ(fuelable->GetFuelLevel(), 25);
	EXPECT_EQ(fuelable->GetFuelCost(), 75);

	EXPECT_DOUBLE_EQ(velocity.m_x, 0);
	EXPECT_DOUBLE_EQ(velocity.m_y, 0);
	EXPECT_DOUBLE_EQ(rotateable->GetAngleVelocity(), M_PI_2);
}

TEST(MacroCommand, RotateNotEnoughFuelLevel) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IRotateable::Ptr rotateable = std::make_shared<RotateableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);
	IExecuteable::Ptr move = std::make_shared<Rotate>(rotateable);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelLevel(25);
	fuelable->SetFuelCost(75);
	rotateable->SetVelocity({ 1, 2 });
	rotateable->SetAngleVelocity(M_PI_2); // 90°

	MacroCommand::CommandList commands{ checkFuel, burnFuel };
	IExecuteable::Ptr macro = std::make_shared<MacroCommand>(commands);

	EXPECT_THROW({ macro->Execute(); }, CommandException);
}

TEST(MacroCommand, ByDirection) {
	UObject::Ptr obj = std::make_shared<UObject>();
	IFuelable::Ptr fuelable = std::make_shared<FuelableAdapter>(obj);
	IMoveable::Ptr moveable = std::make_shared<MoveableAdapter>(obj);
	IDirectionRotateable::Ptr directionRotateable = std::make_shared<DirectionRotateableAdapter>(obj);
	IExecuteable::Ptr checkFuel = std::make_shared<CheckFuelCommand>(fuelable);
	IExecuteable::Ptr move = std::make_shared<Move>(moveable);
	IExecuteable::Ptr directionRotate = std::make_shared<DirectionRotate>(directionRotateable);
	IExecuteable::Ptr changeVelocity = std::make_shared<ChangeVelocityCommand>(moveable, directionRotateable);
	IExecuteable::Ptr burnFuel = std::make_shared<BurnFuelCommand>(fuelable);

	fuelable->SetFuelLevel(100);
	fuelable->SetFuelCost(75);
	moveable->SetPosition({ 1,2 });
	moveable->SetVelocity({ 3,4 });
	directionRotateable->SetDirection({ 0, 1 });
	directionRotateable->SetAngleVelocity(M_PI_2); // 90°

	MacroCommand::CommandList commands{ checkFuel, directionRotate, changeVelocity, move, burnFuel };
	IExecuteable::Ptr macro = std::make_shared<MacroCommand>(commands);

	macro->Execute();

	Vector position = moveable->GetPosition();
	Vector velocity = moveable->GetVelocity();
	Vector direction = directionRotateable->GetDirection();

	EXPECT_EQ(fuelable->GetFuelLevel(), 25);
	EXPECT_EQ(fuelable->GetFuelCost(), 75);

	EXPECT_DOUBLE_EQ(position.m_x, 6);
	EXPECT_DOUBLE_EQ(position.m_y, 2);
	EXPECT_DOUBLE_EQ(velocity.m_x, 5);
	EXPECT_NEAR(velocity.m_y, 0, 1e-12);

	EXPECT_DOUBLE_EQ(direction.m_x, 1);
	EXPECT_NEAR(direction.m_y, 0, 1e-12);
	EXPECT_DOUBLE_EQ(directionRotateable->GetAngleVelocity(), M_PI_2);
}