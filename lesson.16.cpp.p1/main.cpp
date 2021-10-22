#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

#include "Matrix.h"

struct Config {
	std::string output;
	std::vector<std::string> inputs;
};

Config ParseArguments(int argc, char* argv[]) {
	if (argc < 3)
		throw std::runtime_error("Invalid number of arguments");
	std::string output = argv[1];
	std::vector<std::string> inputs;
	for (size_t argn = 2; argn < argc; ++argn)
		inputs.push_back(argv[argn]);
	return Config{ std::move(output), std::move(inputs) };
}

int main(int argc, char* argv[])
{
	try {
		Config config = ParseArguments(argc, argv);

		Matrix matrixSum;
		for (const std::string& input : config.inputs) {
			try {
				matrixSum += Matrix().Read(input);
			}
			catch (std::runtime_error& e) {
				throw std::runtime_error(std::string("Can't read matrix, ") + e.what() + ": " + input);
			}
		}
		try {
			matrixSum.Write(config.output);
		}
		catch (std::runtime_error& e) {
			throw std::runtime_error(std::string("Can't write matrix, ") + e.what() + ": " + config.output);
		}
	}
	catch (std::runtime_error& e) {
		std::cerr << "Erorr: " << e.what() << std::endl;
		std::cerr << "Usage:" << std::endl;
		std::cerr << argv[0] << " ${outputFile} ${inputFile1} [${inputFile2} ... ${inputFileN}]" << std::endl;
		return -1;
	}
	return 0;
}