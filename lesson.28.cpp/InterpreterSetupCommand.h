#pragma once

#include <algorithm>

#include "IoC.h"
#include "LambdaCommand.h"
#include "Tokenizer.h"

#include "..\lesson.03.cpp\IExecuteable.h"

class InterpreterSetupCommand : public IExecuteable
{
protected:
	static bool IsEndOfSection(const std::string& str) {
		return std::find_if(str.begin(), str.end(), [](char c) { return std::isspace(c) == 0; }) == str.end();
	}

public:

	virtual void Execute() override {
		// read tanks file command
		Fabric::Register(
			IoC,
			"Interpreter.Tanks",
			[](Fabric::Args args) {
				std::shared_ptr<std::istream> pInputStream = Fabric::GetRequiredArg<std::shared_ptr<std::istream>>(args, 0, "invalid input stream");
				IExecuteable::Ptr pCommand = std::make_shared<LambdaCommand>([pInputStream] {
					std::string section;
					while (!pInputStream->eof()) {
						std::getline(*pInputStream, section);
						if (section.empty())
							continue;
						IoC->Resolve<IExecuteable::Ptr>(std::string("Interpreter.") + section, { pInputStream })->Execute();
					}
					});
				return pCommand;
			}
		);

		Fabric::Register(
			IoC,
			"Interpreter.SelectStrategies",
			[](Fabric::Args args) {
				std::shared_ptr<std::istream> pInputStream = Fabric::GetRequiredArg<std::shared_ptr<std::istream>>(args, 0, "invalid input stream");
				IExecuteable::Ptr pCommand = std::make_shared<LambdaCommand>([pInputStream] {
					std::string command;
					while (!pInputStream->eof()) {
						std::getline(*pInputStream, command);
						if (IsEndOfSection(command))
							break;

						Tokenizer tokenizer(command);
						std::string strategyType(tokenizer.Next());
						std::string strategyName(tokenizer.Next());
						Fabric::Args strategyArgs;
						while (!tokenizer.IsEndOfString())
							strategyArgs.emplace_back(tokenizer.Next());

						IoC->Resolve<IExecuteable::Ptr>(std::string("TankInitializer.Select.") + strategyType + "." + strategyName, strategyArgs)->Execute();
					}
					});
				return pCommand;
			}
		);

		Fabric::Register(
			IoC,
			"Interpreter.CreateTank",
			[](Fabric::Args args) {
				std::shared_ptr<std::istream> pInputStream = Fabric::GetRequiredArg<std::shared_ptr<std::istream>>(args, 0, "invalid input stream");
				IExecuteable::Ptr pCommand = std::make_shared<LambdaCommand>([pInputStream] {
					UObject::Ptr pUOject = IoC->Resolve<UObject::Ptr>("TankInitializer.Create.Tank");
					std::string command;
					while (!pInputStream->eof()) {
						std::getline(*pInputStream, command);
						if (IsEndOfSection(command))
							break;

						Tokenizer tokenizer(command);
						std::string commandName(tokenizer.Next());
						Fabric::Args commandArgs{ pUOject };
						while (!tokenizer.IsEndOfString())
							commandArgs.emplace_back(tokenizer.Next());

						IoC->Resolve<IExecuteable::Ptr>(std::string("Tank.") + commandName, commandArgs)->Execute();
					}
					});
				return pCommand;
			}
		);
	}
};
