#include <iostream>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <vector>

#include "../lesson.03.cpp/UObject.h"

#include "IoC.h"
#include "TankIntitializerSetupCommand.h"
#include "InterpreterSetupCommand.h"

typedef std::vector<std::filesystem::path> FilePathes;

FilePathes ParseConfig(int argc, char* argv[]) {
	if (argc < 1)
		throw std::runtime_error("Invalid number of arguments");

	FilePathes tanksFiles;
	for (int idx = 1; idx < argc; ++idx)
		tanksFiles.push_back(argv[idx]);

	return tanksFiles;
}

#include "Tokenizer.h"

int main(int argc, char* argv[])
{
	try {
		if (argc < 1)
			throw std::runtime_error("Invalid number of arguments");

		FilePathes tanksFiles = ParseConfig(argc, argv);

		std::make_shared<TankIntitializerSetupCommand>()->Execute();
		std::make_shared < InterpreterSetupCommand>()->Execute();

		for (std::filesystem::path tanksFile : tanksFiles) {
			std::shared_ptr<std::ifstream> pInputFileStream = std::make_shared<std::ifstream>(tanksFile);
			std::shared_ptr<std::istream> pInputStream = pInputFileStream;
			IoC->Resolve<IExecuteable::Ptr>("Interpreter.Tanks", { pInputStream })->Execute();
			pInputFileStream->close();
		}

		UObjectsPtr pTanks = IoC->Resolve<UObjectsPtr>("Globals.Tanks");
		std::cout << "Created " << pTanks->size() << " tanks" << std::endl;
		for (UObject::Ptr pUObject : *pTanks) {
			std::cout << "\t";
			std::cout << "Tank: " << IoC->Resolve<size_t>("Tank.GetIndex", { pUObject }) << "; ";
			std::cout << "Player: " << IoC->Resolve<size_t>("Tank.GetPlayer", { pUObject }) << "; ";
			std::cout << "Position: " << IoC->Resolve<Vector>("Tank.GetPosition", { pUObject }) << "; ";
			std::cout << "Direction: " << IoC->Resolve<Vector>("Tank.GetDirection", { pUObject }) << "; ";
			std::cout << "FuelLevel: " << IoC->Resolve<size_t>("Tank.GetFuelLevel", { pUObject });
			std::cout << std::endl;
		}
	}
	catch (std::runtime_error& e) {
		std::cerr << "Erorr: " << e.what() << std::endl;
		std::cerr << "Usage:" << std::endl;
		std::cerr << argv[0] << " ${tanksFile1} [${tanksFile2} ...  ${tanksFileN}]" << std::endl;
		return -1;
	}
}