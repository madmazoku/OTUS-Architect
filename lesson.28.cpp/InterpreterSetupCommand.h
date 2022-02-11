#pragma once

#include <any>
#include <string>
#include <algorithm>

#include <istream>

#include "../lesson.03.cpp/UObject.h"

#include "LambdaCommand.h"
#include "Tokenizer.h"

#include "IoC.h"

#include "..\lesson.03.cpp\IExecuteable.h"

class InterpreterSetupCommand : public IExecuteable
{
protected:
	static bool IsEndOfSection(const std::string& str) {
		return std::find_if(str.begin(), str.end(), [](char c) { return std::isspace(c) == 0; }) == str.end();
	}

	static std::any Interpret(Fabric::Args args) {
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

	static std::any SelectStrategies(Fabric::Args args) {
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

	static std::any CreateTank(Fabric::Args args) {
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

public:

	virtual void Execute() override {
		// read tanks file command
		Fabric::Register(
			IoC,
			"Interpreter.Interpret",
			InterpreterSetupCommand::Interpret
		);

		Fabric::Register(
			IoC,
			"Interpreter.SelectStrategies",
			InterpreterSetupCommand::SelectStrategies
		);

		Fabric::Register(
			IoC,
			"Interpreter.CreateTank",
			InterpreterSetupCommand::CreateTank
		);
	}
};
