#pragma once

#include <map>

#include "ICompare.h"
#include "ISequence.h"
#include "ISort.h"

#include "../lesson.03.cpp/IExecuteable.h"

// команда заполнения фабрики адаптерами конфиг -> базовый билдер
class ConfigFabricSetupCommand : public IExecuteable
{
protected:
	Fabric::Ptr m_pScope;

public:
	typedef std::vector<std::string> ConfigValue;
	typedef std::map<std::string, ConfigValue> ConfigMap;

	ConfigFabricSetupCommand(Fabric::Ptr pScope) : m_pScope(std::move(pScope)) {}

	virtual void Execute() override {
		AddListOfCompare();
		AddListOfSequence();
		AddListOfSort();
	}
protected:

	static ConfigMap GetConfig(Fabric::Args& args) {
		return Fabric::GetRequiredArg<ConfigMap>(args, 0, "invalid config");
	}

	static Fabric::Ptr GetParent(Fabric::Ptr pScope) {
		Fabric::Ptr pParent = pScope->Resolve< Fabric::Ptr>("Default.Parent");
		if (pParent == nullptr)
			throw std::invalid_argument("No parent scope found");
		return pParent;
	}

	void AddListOfCompare() {
		Fabric::Register(
			m_pScope,
			"Compare.Default",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue compareList = config["Compare"];
				if (compareList.size() != 1)
					throw std::invalid_argument("Single Compare expected");
				return std::any(GetParent(pScope)->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.Default")));
			}
		);
		Fabric::Register(
			m_pScope,
			"Compare.AsNumber",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue compareList = config["Compare"];
				if (compareList.size() != 1)
					throw std::invalid_argument("Single Compare expected");
				return std::any(GetParent(pScope)->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.AsNumber")));
			}
		);
		Fabric::Register(
			m_pScope,
			"Compare.Inverse",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue compareList = config["Compare"];
				std::string compareNext;
				if (compareList.size() == 1)
					compareNext = "Default";
				else if (compareList.size() == 2)
					compareNext = compareList[1];
				else
					throw std::invalid_argument("Inverse expected to be single (Default) or 2 parameter");
				ICompare<std::string>::Ptr pCompare = GetParent(pScope)->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareNext);
				return std::any(GetParent(pScope)->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.Inverse"), { pCompare }));
			}
		);
	}
	void AddListOfSequence() {
		Fabric::Register(
			m_pScope,
			"Sequence.Binary",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue sequenceList = config["Sequence"];
				if (sequenceList.size() != 1)
					throw std::invalid_argument("Single Sequence expected");
				return std::any(GetParent(pScope)->Resolve<ISequence::Ptr>(std::string("Sequence.Binary")));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sequence.Gonnet",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue sequenceList = config["Sequence"];
				if (sequenceList.size() != 1)
					throw std::invalid_argument("Single Sequence expected");
				return std::any(GetParent(pScope)->Resolve<ISequence::Ptr>(std::string("Sequence.Gonnet")));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sequence.Knuth",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue sequenceList = config["Sequence"];
				if (sequenceList.size() != 1)
					throw std::invalid_argument("Single Sequence expected");
				return std::any(GetParent(pScope)->Resolve<ISequence::Ptr>(std::string("Sequence.Knuth")));
			}
		);
	}
	void AddListOfSort() {
		Fabric::Register(
			m_pScope,
			"Sort.Buble",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue SortList = config["Sort"];
				if (SortList.size() != 1)
					throw std::invalid_argument("Single Sort expected");
				ConfigValue compareList = config["Compare"];
				ICompare<std::string>::Ptr pCompare = pScope->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], { config });
				return std::any(GetParent(pScope)->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Buble"), { pCompare }));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Heap",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue SortList = config["Sort"];
				if (SortList.size() != 1)
					throw std::invalid_argument("Single Sort expected");
				ConfigValue compareList = config["Compare"];
				ICompare<std::string>::Ptr pCompare = pScope->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], { config });
				return std::any(GetParent(pScope)->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Heap"), { pCompare }));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Insertion",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue SortList = config["Sort"];
				if (SortList.size() != 1)
					throw std::invalid_argument("Single Sort expected");
				ConfigValue compareList = config["Compare"];
				ICompare<std::string>::Ptr pCompare = pScope->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], { config });
				return std::any(GetParent(pScope)->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Insertion"), { pCompare }));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Merge",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue SortList = config["Sort"];
				if (SortList.size() != 1)
					throw std::invalid_argument("Single Sort expected");
				ConfigValue compareList = config["Compare"];
				ICompare<std::string>::Ptr pCompare = pScope->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], { config });
				return std::any(GetParent(pScope)->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Merge"), { pCompare }));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Quick",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue SortList = config["Sort"];
				if (SortList.size() != 1)
					throw std::invalid_argument("Single Sort expected");
				ConfigValue compareList = config["Compare"];
				ICompare<std::string>::Ptr pCompare = pScope->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], { config });
				return std::any(GetParent(pScope)->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Quick"), { pCompare }));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Selection",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue SortList = config["Sort"];
				if (SortList.size() != 1)
					throw std::invalid_argument("Single Sort expected");
				ConfigValue compareList = config["Compare"];
				ICompare<std::string>::Ptr pCompare = pScope->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], { config });
				return std::any(GetParent(pScope)->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Selection"), { pCompare }));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Shell",
			[pScope = m_pScope](Fabric::Args args) {
				ConfigMap config = GetConfig(args);
				ConfigValue SortList = config["Sort"];
				if (SortList.size() != 1)
					throw std::invalid_argument("Single Sort expected");
				ConfigValue compareList = config["Compare"];
				ICompare<std::string>::Ptr pCompare = pScope->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], { config });
				ConfigValue sequenceList = config["Sequence"];
				ISequence::Ptr pSequence = pScope->Resolve<ISequence::Ptr>(std::string("Sequence.") + sequenceList[0], { config });
				return std::any(GetParent(pScope)->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Shell"), { pCompare, pSequence }));
			}
		);
	}
};