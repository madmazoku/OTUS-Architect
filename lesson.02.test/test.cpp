#include "pch.h"

#include "../lesson.02.cpp/Positional.h"
#include "../lesson.02.cpp/MoveByVector.h"

TEST(MoveObject, MoveObjectSuccess) {
	Positional object({ 12,5 });
	MoveByVector move(object, { -7, 3 });
	move.Execute(1);
	EXPECT_EQ(object.m_pos.m_x, 5);
	EXPECT_EQ(object.m_pos.m_y, 8);
}