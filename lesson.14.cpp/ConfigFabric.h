#pragma once

#include <map>

#include "Fabric.h"

class ConfigFabric : public Fabric
{
public:
	typedef std::vector<std::string> ConfigValue;
	typedef std::map<std::string, ConfigValue> ConfigMap;

	ConfigFabric(Fabric::Ptr pParent) : Fabric(pParent) {
		AddListOfCompare();
		AddListOfSequence();
		AddListOfSort();
	}
protected:

	std::map<std::string, std::vector<std::string>> GetConfig(Args args) const {
		if (args.size() != 1)
			throw std::invalid_argument("invalid arguments number");
		std::map<std::string, std::vector<std::string>> config;
		try {
			config = std::any_cast<std::map<std::string, std::vector<std::string>>>(args[0]);
		}
		catch (std::bad_any_cast e) {
			throw std::invalid_argument("invalid argument type");
		}
		return config;
	}

	void AddListOfCompare() {
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Compare.Default"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue compareList = config["Compare"];
					if (compareList.size() != 1)
						throw std::invalid_argument("Single Compare expected");
					return std::any(m_pParent->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.Default")));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Compare.AsNumber"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue compareList = config["Compare"];
					if (compareList.size() != 1)
						throw std::invalid_argument("Single Compare expected");
					return std::any(m_pParent->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.AsNumber")));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Compare.Inverse"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue compareList = config["Compare"];
					std::string compareNext;
					if (compareList.size() == 1)
						compareNext = "Default";
					else if (compareList.size() == 2)
						compareNext = compareList[1];
					else
						throw std::invalid_argument("Inverse expected to be single (Default) or 2 parameter");
					ICompare<std::string>::Ptr pCompare = m_pParent->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareNext);
					return std::any(m_pParent->Resolve<ICompare<std::string>::Ptr>(std::string("Compare.Inverse"), {pCompare}));
				}
			}
		)->Do();
	}
	void AddListOfSequence() {
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sequence.Binary"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue sequenceList = config["Sequence"];
					if (sequenceList.size() != 1)
						throw std::invalid_argument("Single Sequence expected");
					return std::any(m_pParent->Resolve<ISequence::Ptr>(std::string("Sequence.Binary")));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sequence.Gonnet"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue sequenceList = config["Sequence"];
					if (sequenceList.size() != 1)
						throw std::invalid_argument("Single Sequence expected");
					return std::any(m_pParent->Resolve<ISequence::Ptr>(std::string("Sequence.Gonnet")));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sequence.Knuth"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue sequenceList = config["Sequence"];
					if (sequenceList.size() != 1)
						throw std::invalid_argument("Single Sequence expected");
					return std::any(m_pParent->Resolve<ISequence::Ptr>(std::string("Sequence.Knuth")));
				}
			}
		)->Do();
	}
	void AddListOfSort() {
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Buble"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue SortList = config["Sort"];
					if (SortList.size() != 1)
						throw std::invalid_argument("Single Sort expected");
					ConfigValue compareList = config["Compare"];
					ICompare<std::string>::Ptr pCompare = Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], {config});
					return std::any(m_pParent->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Buble"), {pCompare}));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Heap"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue SortList = config["Sort"];
					if (SortList.size() != 1)
						throw std::invalid_argument("Single Sort expected");
					ConfigValue compareList = config["Compare"];
					ICompare<std::string>::Ptr pCompare = Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], {config});
					return std::any(m_pParent->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Heap"), {pCompare}));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Insertion"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue SortList = config["Sort"];
					if (SortList.size() != 1)
						throw std::invalid_argument("Single Sort expected");
					ConfigValue compareList = config["Compare"];
					ICompare<std::string>::Ptr pCompare = Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], {config});
					return std::any(m_pParent->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Insertion"), {pCompare}));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Merge"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue SortList = config["Sort"];
					if (SortList.size() != 1)
						throw std::invalid_argument("Single Sort expected");
					ConfigValue compareList = config["Compare"];
					ICompare<std::string>::Ptr pCompare = Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], {config});
					return std::any(m_pParent->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Merge"), {pCompare}));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Quick"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue SortList = config["Sort"];
					if (SortList.size() != 1)
						throw std::invalid_argument("Single Sort expected");
					ConfigValue compareList = config["Compare"];
					ICompare<std::string>::Ptr pCompare = Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], {config});
					return std::any(m_pParent->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Quick"), {pCompare}));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Selection"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue SortList = config["Sort"];
					if (SortList.size() != 1)
						throw std::invalid_argument("Single Sort expected");
					ConfigValue compareList = config["Compare"];
					ICompare<std::string>::Ptr pCompare = Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], {config});
					return std::any(m_pParent->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Selection"), {pCompare}));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Shell"),
				(Lambda)[&](Fabric::Args args) {
					ConfigMap config = GetConfig(args);
					ConfigValue SortList = config["Sort"];
					if (SortList.size() != 1)
						throw std::invalid_argument("Single Sort expected");
					ConfigValue compareList = config["Compare"];
					ICompare<std::string>::Ptr pCompare = Resolve<ICompare<std::string>::Ptr>(std::string("Compare.") + compareList[0], {config});
					ConfigValue sequenceList = config["Sequence"];
					ISequence::Ptr pSequence = Resolve<ISequence::Ptr>(std::string("Sequence.") + sequenceList[0], { config });
					return std::any(m_pParent->Resolve<ISort<std::string>::Ptr>(std::string("Sort.Shell"), {pCompare, pSequence}));
				}
			}
		)->Do();
	}
};