#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "RandomMatrix.h"

#include "../lesson.03.cpp/UObject.h"
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
	for (size_t argn = 4; argn < argc; ++argn)
		randoms.push_back(argv[argn]);

	return Config{ columns, rows, std::move(summary), std::move(randoms) };
}

int main(int argc, char* argv[])
{
	std::cerr << std::filesystem::current_path() << std::endl;
	try {
		Config config = ParseArguments(argc, argv);
		for (const std::string& matrix : config.matrixes) {
			try {
				RandomMatrix().Generate(config.columns, config.rows).Write(matrix);
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