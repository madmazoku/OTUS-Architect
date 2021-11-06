#include "pch.h"

#include <sstream>

#include "../lesson.16.cpp.p1/Matrix.h"

TEST(Matrix, Read) {
	std::stringstream ss("1 2\n3\t4\n5 \t 6");
	Matrix matrixExpect({ 1,2,3,4,5,6 }, 2, 3);
	Matrix matrixGot;
	matrixGot.Read(ss);
	EXPECT_TRUE(matrixGot.Compare(matrixExpect, 1e-6));
}

TEST(Matrix, Write) {
	std::stringstream ss;
	Matrix matrix({ 1,2,3,4,5,6 }, 2, 3);
	matrix.Write(ss);
	EXPECT_EQ(ss.str(), "1\t2\n3\t4\n5\t6\n");
}

TEST(Matrix, Sum) {
	std::stringstream ss;
	Matrix matrixGot({ 1,2,3,4,5,6 }, 2, 3);
	Matrix matrixAdd({ 10,20,30,40,50,60 }, 2, 3);
	Matrix matrixExpect({ 11,22,33,44,55,66 }, 2, 3);
	matrixGot += matrixAdd;
	EXPECT_TRUE(matrixGot.Compare(matrixExpect, 1e-6));
}

TEST(Matrix, Compare) {
	std::stringstream ss;
	Matrix matrix({ 1,2,3,4,5,6 }, 2, 3);
	Matrix matrixEq({ 1,2,3,4,5,6 }, 2, 3);
	Matrix matrixNotEq({ 11,22,33,44,55,66 }, 2, 3);
	EXPECT_TRUE(matrix.Compare(matrixEq, 1e-6));
	EXPECT_TRUE(!matrix.Compare(matrixNotEq, 1e-6));
}