#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <sstream>
#include <regex>
#include <vector>

#include "Parser.h"

const std::regex g_reAutoGenerate("^//!AutoGenerate:\"(.+)\"$");

int main(int argc, char* argv[])
{
	try {
		if (argc < 2)
			throw std::runtime_error("Input directory not set");
		std::filesystem::path adapterDir(argv[1]);
		if (!std::filesystem::is_directory(adapterDir))
			throw std::runtime_error(std::string("Input directory ") + adapterDir.string() + std::string(" absent"));

		for (const std::filesystem::directory_entry& de : std::filesystem::recursive_directory_iterator(adapterDir)) {
			if (!std::filesystem::is_regular_file(de.path()))
				continue;
			if (de.path().extension() != ".h")
				continue;
			std::cout << "Consider " << de.path() << std::endl;
			std::ifstream adapterStream(de.path());
			std::string line;
			std::getline(adapterStream, line);
			adapterStream.close();
			std::smatch m;
			if (!std::regex_match(line, m, g_reAutoGenerate))
				continue;

			std::string strInterfaceHeader(m[1].str());
			std::filesystem::path outputFile(de.path());
			std::filesystem::path inputFile(outputFile.parent_path() / strInterfaceHeader);
			if (!std::filesystem::is_regular_file(inputFile))
				throw std::runtime_error(
					std::string("invalid input file path ") +
					inputFile.string() +
					std::string(" for autogenerate ") +
					outputFile.string()
				);

			std::cout << "Autogenerate IoC UObject adapter from " << inputFile << std::endl;
			std::filesystem::path outputFileTmp(outputFile.string() + ".tmp");
			if (std::filesystem::exists(outputFileTmp))
				std::filesystem::remove(outputFileTmp);

			std::ifstream inputStream(inputFile);
			std::ofstream outputStream(outputFileTmp);
			outputStream << line << "\n";
			outputStream << "#pragma once\n";
			outputStream << "#include \"" << strInterfaceHeader << "\"" << std::endl;
			Translate(inputStream, outputStream);
			outputStream.close();
			inputStream.close();

			std::filesystem::rename(outputFileTmp, outputFile);
		}
	}
	catch (std::runtime_error& e) {
		std::cerr << "Erorr: " << e.what() << std::endl;
		std::cerr << "Usage:" << std::endl;
		std::cerr << argv[0] << " ${adapterDir}" << std::endl;
		return -1;
	}
}