#include "pch.h"

#include <filesystem>

#include "../lesson.16.cpp.p2/RandomMatrix.h"
#include "../lesson.16.cpp.p2/FileMatrixSumableAdapter.h"
#include "../lesson.16.cpp.p2/FileMatrixSumCommand.h"

const std::string g_projectName(PROJECT_NAME);

TEST(FileMatrixSumCommand, Success) {
	std::filesystem::path testPath(g_projectName);
	std::string summaryGot = (testPath / "sum_1_2_got.txt").string();
	std::string summaryExpect = (testPath / "sum_1_2_expect.txt").string();
	std::vector<std::string> matrixes;
	matrixes.push_back((testPath / "matrix1.txt").string());
	matrixes.push_back((testPath / "matrix2.txt").string());

	UObject::Ptr pUObject = std::make_shared<UObject>();
	IFileMatrixSumable::Ptr pAdapter = std::make_shared<FileMatrixSumableAdapter>(pUObject);
	IExecuteable::Ptr pCommand = std::make_shared<FileMatrixSumCommand>(pAdapter);

	pAdapter->SetSummary(summaryGot);
	pAdapter->SetMatrixes(matrixes);
	pCommand->Execute();

	Matrix matrixGot;
	Matrix matrixExpect;
	matrixGot.Read(summaryGot);
	matrixExpect.Read(summaryExpect);

	EXPECT_TRUE(matrixGot.Compare(matrixExpect, 1e-6));
}