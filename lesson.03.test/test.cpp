#include "pch.h"

#include <cmath>

#include "../Common/Vector.h"
#include "../lesson.03.cpp/UObject.h"
#include "../lesson.03.cpp/IExecuteable.h"
#include "../lesson.03.cpp/MoveableAdapter.h"
#include "../lesson.03.cpp/Move.h"
#include "../lesson.03.cpp/RotateableAdapter.h"
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

	EXPECT_THROW({pUObject->GetProperty("position");}, std::invalid_argument);
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

TEST(Rotate, Success) {
	UObject::Ptr pUObject = std::make_shared<UObject>();

	pUObject->SetProperty("velocity", Vector(0, 1));
	pUObject->SetProperty("angleVelocity", M_PI_2);

	IExecuteable::Ptr pRotate = std::make_shared<Rotate>(std::make_shared<RotateableAdapter>(pUObject));
	pRotate->Execute();

	Vector velocity = std::any_cast<Vector>(pUObject->GetProperty("velocity"));

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