#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <random>
#include <cmath>

#include "../lesson.03.cpp/UObject.h"
#include "../lesson.16.cpp.p1/Matrix.h"
#include "FileMatrixSumableAdapter.h"
#include "FileMatrixSumCommand.h"

struct Config {
	size_t columns;
	size_t rows;
	std::string summary;
	std::vector<std::string> matrixes;
};

Config ParseArguments(int argc, char* argv[]) {
	if (argc < 5)
		throw std::runtime_error("Invalid number of arguments");

	size_t columns = std::stol(argv[1]);
	size_t rows = std::stol(argv[2]);
	std::string summary = argv[3];
	std::vector<std::string> randoms;
	for (int argn = 4; argn < argc; ++argn)
		randoms.emplace_back(argv[argn]);

	return Config{ columns, rows, std::move(summary), std::move(randoms) };
}

Matrix RandomMatrixGenerate(size_t columns, size_t rows) {
	std::vector<double> data;

	std::random_device rd;
	std::default_random_engine re(rd());
	std::uniform_int_distribution<int> ud_x(-10000, 10000);
	while (data.size() != columns * rows)
		data.emplace_back(0.01 * ud_x(re));

	return Matrix(data, columns, rows);
}

int main(int argc, char* argv[])
{
	std::cerr << std::filesystem::current_path() << std::endl;
	try {
		Config config = ParseArguments(argc, argv);
		for (const std::string& matrix : config.matrixes) {
			try {
				RandomMatrixGenerate(config.columns, config.rows).Write(matrix);
			}
			catch (std::runtime_error& e) {
				throw std::runtime_error(std::string("Can't generate matrix, ") + e.what() + ": " + matrix);
			}
		}

		UObject::Ptr pUObject = std::make_shared<UObject>();
		IFileMatrixSumable::Ptr pAdapter = std::make_shared<FileMatrixSumableAdapter>(pUObject);
		IExecuteable::Ptr pCommand = std::make_shared<FileMatrixSumCommand>(pAdapter);

		pAdapter->SetSummary(config.summary);
		pAdapter->SetMatrixes(config.matrixes);
		pCommand->Execute();
	}
	catch (std::runtime_error& e) {
		std::cerr << "Erorr: " << e.what() << std::endl;
		std::cerr << "Usage:" << std::endl;
		std::cerr << argv[0] << " ${columns} ${rows} ${summaryFile} ${generateMatrixFile1} [${generateMatrixFile2} ... ${generateMatrixFileN}]" << std::endl;
		return -1;
	}
}