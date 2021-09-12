#include "pch.h"

#include <cmath>

#include "../Common/Vector.h"
#include "../lesson.03.cpp/UObject.h"
#include "../lesson.03.cpp/IExecuteable.h"
#include "../lesson.03.cpp/MoveableAdapter.h"
#include "../lesson.03.cpp/UnMoveableAdapter.h"
#include "../lesson.03.cpp/Move.h"
#include "../lesson.03.cpp/RotateableAdapter.h"
#include "../lesson.03.cpp/UnRotateableAdapter.h"
#include "../lesson.03.cpp/Rotate.h"

TEST(UObject, Property) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("position", Vector(1, 2));
	Vector position = std::any_cast<Vector>(pUObject->GetProperty("position"));

	EXPECT_DOUBLE_EQ(position.m_x, 1);
	EXPECT_DOUBLE_EQ(position.m_y, 2);
}

TEST(UObject, PropertyNotSet) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	EXPECT_THROW({ pUObject->GetProperty("position"); }, std::invalid_argument);
}

TEST(UObject, PropertyRemove) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("position", Vector(1, 2));
	pUObject->RemoveProperty("position");

	EXPECT_THROW({ pUObject->GetProperty("position"); }, std::invalid_argument);
}

TEST(UObject, PropertyRemoveNotSet) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	EXPECT_THROW({ pUObject->RemoveProperty("position"); }, std::invalid_argument);
}

TEST(UObject, PropertyReadonly) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("position", Vector(1, 2), true);

	EXPECT_THROW({ pUObject->SetProperty("position", Vector(2,1)); }, std::invalid_argument);
}

TEST(UObject, NoPropertySetReadonly) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	EXPECT_THROW({ pUObject->SetReadonly("position"); }, std::invalid_argument);
}

TEST(UObject, PropertySetReadonly) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("position", Vector(1, 2));
	pUObject->SetReadonly("position");

	EXPECT_THROW({ pUObject->SetProperty("position", Vector(2,1)); }, std::invalid_argument);
}

TEST(UObject, PropertyRemoveReadonly) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("position", Vector(1, 2), true);
	pUObject->SetReadonly("position", false);
	pUObject->SetProperty("position", Vector(2, 1));

	Vector position = std::any_cast<Vector>(pUObject->GetProperty("position"));

	EXPECT_DOUBLE_EQ(position.m_x, 2);
	EXPECT_DOUBLE_EQ(position.m_y, 1);
}

TEST(Move, Success) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("position", Vector(12, 5));
	pUObject->SetProperty("velocity", Vector(-7, 3));

	IExecuteable::Ptr pMove = std::make_shared<Move>(std::make_shared<MoveableAdapter>(pUObject));
	pMove->Execute();

	Vector position = std::any_cast<Vector>(pUObject->GetProperty("position"));

	EXPECT_DOUBLE_EQ(position.m_x, 5);
	EXPECT_DOUBLE_EQ(position.m_y, 8);
}

TEST(Move, NoPosition) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("velocity", Vector(-7, 3));

	IExecuteable::Ptr pMove = std::make_shared<Move>(std::make_shared<MoveableAdapter>(pUObject));

	EXPECT_THROW({ pMove->Execute(); }, std::invalid_argument);
}

TEST(Move, NoVelocity) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("position", Vector(12, 5));

	IExecuteable::Ptr pMove = std::make_shared<Move>(std::make_shared<MoveableAdapter>(pUObject));

	EXPECT_THROW({ pMove->Execute(); }, std::invalid_argument);
}

TEST(Move, ReadonlyPosition) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("position", Vector(12, 5), true);
	pUObject->SetProperty("velocity", Vector(-7, 3));

	IExecuteable::Ptr pMove = std::make_shared<Move>(std::make_shared<MoveableAdapter>(pUObject));

	EXPECT_THROW({ pMove->Execute(); }, std::invalid_argument);
}

TEST(Move, UnMoveable) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("position", Vector(12, 5), true);
	pUObject->SetProperty("velocity", Vector(-7, 3));

	IExecuteable::Ptr pMove = std::make_shared<Move>(std::make_shared<UnMoveableAdapter>(pUObject));

	EXPECT_THROW({ pMove->Execute(); }, std::invalid_argument);
}

TEST(Rotate, Success) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("velocity", Vector(0, 1));
	pUObject->SetProperty("angleVelocity", M_PI_4);

	IExecuteable::Ptr pRotate = std::make_shared<Rotate>(std::make_shared<RotateableAdapter>(pUObject));
	pRotate->Execute(); // 45°
	pRotate->Execute(); // 45°

	Vector velocity = std::any_cast<Vector>(pUObject->GetProperty("velocity")); // 90°

	EXPECT_NEAR(velocity.m_x, 1, 1e-15);
	EXPECT_NEAR(velocity.m_y, 0, 1e-15);
}

TEST(Rotate, NoVelocity) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("angleVelocity", M_PI_2);

	IExecuteable::Ptr pRotate = std::make_shared<Rotate>(std::make_shared<RotateableAdapter>(pUObject));

	EXPECT_THROW({ pRotate->Execute(); }, std::invalid_argument);
}

TEST(Rotate, NoAngleVelocity) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("velocity", Vector(0, 1));

	IExecuteable::Ptr pRotate = std::make_shared<Rotate>(std::make_shared<RotateableAdapter>(pUObject));

	EXPECT_THROW({ pRotate->Execute(); }, std::invalid_argument);
}

TEST(Rotate, ReadonlyVelocity) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("velocity", Vector(0, 1), true);
	pUObject->SetProperty("angleVelocity", M_PI_2);

	IExecuteable::Ptr pRotate = std::make_shared<Rotate>(std::make_shared<RotateableAdapter>(pUObject));

	EXPECT_THROW({ pRotate->Execute(); }, std::invalid_argument);
}

TEST(Rotate, UnRotateable) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("velocity", Vector(0, 1), true);
	pUObject->SetProperty("angleVelocity", M_PI_2);

	IExecuteable::Ptr pRotate = std::make_shared<Rotate>(std::make_shared<UnRotateableAdapter>(pUObject));

	EXPECT_THROW({ pRotate->Execute(); }, std::invalid_argument);
}