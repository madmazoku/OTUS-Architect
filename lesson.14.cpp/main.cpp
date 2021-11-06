#include <iostream>
#include <fstream>

#include "SelectionSort.h"
#include "InsertionSort.h"
#include "MergeSort.h"
#include "BubbleSort.h"
#include "HeapSort.h"
#include "QuickSort.h"
#include "ShellSort.h"

#include "BinarySequence.h"
#include "GonnetSequence.h"
#include "KnuthSequence.h"

#include "DefaultCompare.h"
#include "InverseCompare.h"

#include "Fabric.h"
#include "ICommand.h"

#include "AppFabricSetupCommand.h"
#include "ConfigFabricSetupCommand.h"

ConfigFabricSetupCommand::ConfigMap ParseConfig(int argc, char* argv[]) {
	ConfigFabricSetupCommand::ConfigMap config;

	config["Compare"] = { "Default" };
	config["Sort"] = { "Quick" };
	config["Sequence"] = { "Knuth" };
	config["Head"] = { std::to_string(std::numeric_limits<size_t>::max()) };

	int idx = 0;
	while (idx < argc) {
		std::string field;
		if (argv[idx][0] == '-') {
			field = std::string(argv[idx] + 1);
			++idx;
		}
		else
			field = "Unassigned";
		std::vector<std::string> values;
		while (idx < argc && argv[idx][0] != '-')
			values.push_back(std::string(argv[idx++]));
		config[field] = values;
	}

	return config;
}

void Validate(ConfigFabricSetupCommand::ConfigMap config) {
	if (config["Sort"].size() != 1)
		throw std::invalid_argument("Single Sort expected");
	if (config["Input"].empty())
		throw std::invalid_argument("At least one Input expected");
	if (config["Output"].size() != 1)
		throw std::invalid_argument("Single Output expected");
	if (config["Head"].size() != 1)
		throw std::invalid_argument("Single Head expected");
}

std::vector<std::string> ReadData(ConfigFabricSetupCommand::ConfigMap config) {
	size_t head = std::stoull(config["Head"][0]);
	std::vector<std::string> data;
	for (std::string& inputFile : config["Input"]) {
		std::ifstream inputStream(inputFile);
		for (std::string line; data.size() < head && std::getline(inputStream, line);)
			data.push_back(line);
		inputStream.close();
	}
	return data;
}

void WriteData(std::string sortName, ConfigFabricSetupCommand::ConfigMap config, std::vector<std::string>& data) {
	std::ofstream outputStream(config["Output"][0]);
	outputStream << "Config" << std::endl;
	for (ConfigFabricSetupCommand::ConfigMap::value_type& p : config) {
		outputStream << "\t" << p.first << std::endl;
		for (std::string& s : p.second)
			outputStream << "\t\t" << s << std::endl;
	}
	outputStream << "Sort by: " << sortName << std::endl;
	outputStream << std::string(80, '=') << std::endl;

	for (std::string& s : data)
		outputStream << s << std::endl;
	outputStream.close();
}

int DoCmd(int argc, char* argv[]) {
	ConfigFabricSetupCommand::ConfigMap config = ParseConfig(argc, argv);
	if (config.find("Help") != config.end() ||
		config.find("Input") == config.end() ||
		config.find("Output") == config.end())
	{
		std::cout << "Call format:" << std::endl;
		std::cout << "\t" << config["Unassigned"][0] << std::endl;
		std::cout << "\t\t-Input ${path to input file 1} [${path to input file 2} ... ${path to input file N}]" << std::endl;
		std::cout << "\t\t-Output ${path to output file}" << std::endl;
		std::cout << "\t\t[-Head ${how much lines to read; default: all of them}]" << std::endl;
		std::cout << "\t\t[-Sort ${optional sort type: Bubble, Heap, Insertion, Merge, Quick, Selection, Shell; default: Quick}]" << std::endl;
		std::cout << "\t\t[-Compare ${optional compare type: AsNumber, Default, Inverse; Inverese may be fillowed by another compare type, it's Default by default}]" << std::endl;
		std::cout << "\t\t[-Sequence ${optional sequence type: Binary, Gonnet, Knuth; default: Knuth; need for Shell sort only}]" << std::endl;
		std::cout << std::endl;
		std::cout << "Example:" << std::endl;
		std::cout << "\t" << config["Unassigned"][0] << " -Input c:\\input.txt -Output c:\\output.txt -Sort Shell -Compare Inverse AsNumber -Sequence Gonnet" << std::endl;
		return 1;
	}

	Validate(config);

	Fabric::Ptr pRootFabric = std::make_shared<Fabric>(nullptr);
	Fabric::Ptr pAppFabric = pRootFabric->Resolve<Fabric::Ptr>("Default.NewScope", { pRootFabric });
	std::make_shared<AppFabricSetupCommand>(pAppFabric)->Do();
	Fabric::Ptr pConfigFabric = pAppFabric->Resolve<Fabric::Ptr>("Default.NewScope", { pAppFabric });
	std::make_shared<ConfigFabricSetupCommand>(pConfigFabric)->Do();

	ISort<std::string>::Ptr pSort = pConfigFabric->Resolve< ISort<std::string>::Ptr>(std::string("Sort.") + config["Sort"][0], { config });
	std::vector<std::string> data = ReadData(config);
	pSort->Sort(data);
	WriteData(pSort->Name(), config, data);

	return 0;
}

void DoSortSimpleChecks()
{
	std::vector<int> items{ 2,4,6,5,3,1 };

	std::cout << "Before sort:" << std::endl;
	for (int i : items)
		std::cout << i << std::endl;

	std::vector<ICompare<int>::Ptr> Compares;
	Compares.push_back(std::make_shared<DefaultCompare<int>>());
	Compares.push_back(std::make_shared<InverseCompare<int>>(std::make_shared<DefaultCompare<int>>()));

	std::vector<ISequence::Ptr> sequences;
	sequences.push_back(std::make_shared<BinarySequence>());
	sequences.push_back(std::make_shared<GonnetSequence>());
	sequences.push_back(std::make_shared<KnuthSequence>());

	std::vector<ISort<int>::Ptr> sorts;
	for (auto pCompare : Compares) {
		sorts.push_back(std::make_shared<BubbleSort<int>>(pCompare));
		sorts.push_back(std::make_shared<HeapSort<int>>(pCompare));
		sorts.push_back(std::make_shared<InsertionSort<int>>(pCompare));
		sorts.push_back(std::make_shared<MergeSort<int>>(pCompare));
		sorts.push_back(std::make_shared<QuickSort<int>>(pCompare));
		sorts.push_back(std::make_shared<SelectionSort<int>>(pCompare));
		for (auto pSequence : sequences)
			sorts.push_back(std::make_shared<ShellSort<int>>(pCompare, pSequence));
	}

	for (auto pSort : sorts) {
		std::vector<int> sorted(items);
		std::cout << pSort->Name() << std::endl;
		pSort->Sort(sorted);
		for (int i : sorted)
			std::cout << i << std::endl;
	}
}

void DoSortSelectionChecks() {
	Fabric::Ptr pRootFabric = std::make_shared<Fabric>(nullptr);
	Fabric::Ptr pAppFabric = pRootFabric->Resolve<Fabric::Ptr>("Default.NewScope", { pRootFabric });
	std::make_shared<AppFabricSetupCommand>(pAppFabric)->Do();
	Fabric::Ptr pConfigFabric = pAppFabric->Resolve<Fabric::Ptr>("Default.NewScope", { pAppFabric });
	std::make_shared<ConfigFabricSetupCommand>(pConfigFabric)->Do();

	ConfigFabricSetupCommand::ConfigMap config;
	config["Compare"] = { "Inverse" };
	config["Sort"] = { "Quick" };
	config["Sequence"] = { "Knuth" };
	config["Head"] = { std::to_string(std::numeric_limits<size_t>::max()) };

	ISort<std::string>::Ptr pSort = pConfigFabric->Resolve< ISort<std::string>::Ptr>(std::string("Sort.") + config["Sort"][0], { config });
	std::vector<std::string> data{ "2","4","6","111", "5","3","1" };

	pSort->Sort(data);

	std::cout << pSort->Name() << std::endl;
	pSort->Sort(data);
	for (std::string& line : data)
		std::cout << line << std::endl;
}

int main(int argc, char* argv[]) {
	DoSortSelectionChecks();
	//return DoCmd(argc, argv);
}