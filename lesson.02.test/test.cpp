#include "pch.h"

#include <math.h>

#include "../lesson.02.cpp/Positional.h"
#include "../lesson.02.cpp/MoveByVector.h"
#include "../lesson.02.cpp/Directional.h"
#include "../lesson.02.cpp/RotateByAngle.h"

TEST(Objects, MoveByVector) {
	Positional object({ 12,5 });
	MoveByVector move(object, { -7, 3 });
	move.Execute(1);
	EXPECT_DOUBLE_EQ(object.m_pos.m_x, 5);
	EXPECT_DOUBLE_EQ(object.m_pos.m_y, 8);
}

TEST(Objects, DirectionalNormalize) {
	Directional object({ 1,2 });
	double invLen = 1.0 / sqrt(5.0);
	EXPECT_DOUBLE_EQ(object.m_dir.m_x, invLen);
	EXPECT_DOUBLE_EQ(object.m_dir.m_y, 2 * invLen);
}

TEST(Objects, RotateByAngle) {
	Directional object({ sin(0.5), cos(0.5) });
	RotateByAngle rotate(object, 0.25);
	rotate.Execute(1);
	EXPECT_DOUBLE_EQ(object.m_dir.m_x, sin(0.75));
	EXPECT_DOUBLE_EQ(object.m_dir.m_y, cos(0.75));
}