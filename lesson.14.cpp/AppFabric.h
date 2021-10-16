#pragma once

#include "Fabric.h"
#include "ICommand.h"

#include "ICompare.h"
#include "DefaultCompare.h"
#include "InverseCompare.h"
#include "AsNumberCompare.h"

#include "ISequence.h"
#include "BinarySequence.h"
#include "GonnetSequence.h"
#include "KnuthSequence.h"

class AppFabric : public Fabric
{
public:
	AppFabric(Fabric::Ptr pParent = nullptr) : Fabric(pParent) {
		AddListOfCompare();
		AddListOfSequence();
		AddListOfSort();
	}

protected:
	void AddListOfCompare() {
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Compare.Default"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 0)
						throw std::invalid_argument("invalid arguments number");
					return std::any((ICompare<std::string>::Ptr)std::make_shared<DefaultCompare<std::string>>());
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Compare.AsNumber"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 0)
						throw std::invalid_argument("invalid arguments number");
					return std::any((ICompare<std::string>::Ptr)std::make_shared<AsNumberCompare>());
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Compare.Inverse"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 1)
						throw std::invalid_argument("invalid arguments number");
					ICompare<std::string>::Ptr pCompare;
					try {
						pCompare = std::any_cast<ICompare<std::string>::Ptr>(args[0]);
					}
					catch (std::bad_any_cast e) {
						throw std::invalid_argument("invalid argument type");
					}
					return std::any((ICompare<std::string>::Ptr)std::make_shared<InverseCompare<std::string>>(pCompare));
				}
			}
		)->Do();
	}
	void AddListOfSequence() {
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sequence.Binary"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 0)
						throw std::invalid_argument("invalid arguments number");
					return std::any((ISequence::Ptr)std::make_shared<BinarySequence>());
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sequence.Gonnet"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 0)
						throw std::invalid_argument("invalid arguments number");
					return std::any((ISequence::Ptr)std::make_shared<GonnetSequence>());
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sequence.Knuth"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 0)
						throw std::invalid_argument("invalid arguments number");
					return std::any((ISequence::Ptr)std::make_shared<KnuthSequence>());
				}
			}
		)->Do();
	}
	void AddListOfSort() {
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Buble"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 1)
						throw std::invalid_argument("invalid arguments number");
					ICompare<std::string>::Ptr pCompare;
					try {
						pCompare = std::any_cast<ICompare<std::string>::Ptr>(args[0]);
					}
					catch (std::bad_any_cast e) {
						throw std::invalid_argument("invalid argument type");
					}
					return std::any((ISort<std::string>::Ptr)std::make_shared<BubbleSort<std::string>>(pCompare));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Heap"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 1)
						throw std::invalid_argument("invalid arguments number");
					ICompare<std::string>::Ptr pCompare;
					try {
						pCompare = std::any_cast<ICompare<std::string>::Ptr>(args[0]);
					}
					catch (std::bad_any_cast e) {
						throw std::invalid_argument("invalid argument type");
					}
					return std::any((ISort<std::string>::Ptr)std::make_shared<HeapSort<std::string>>(pCompare));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Insertion"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 1)
						throw std::invalid_argument("invalid arguments number");
					ICompare<std::string>::Ptr pCompare;
					try {
						pCompare = std::any_cast<ICompare<std::string>::Ptr>(args[0]);
					}
					catch (std::bad_any_cast e) {
						throw std::invalid_argument("invalid argument type");
					}
					return std::any((ISort<std::string>::Ptr)std::make_shared<InsertionSort<std::string>>(pCompare));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Merge"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 1)
						throw std::invalid_argument("invalid arguments number");
					ICompare<std::string>::Ptr pCompare;
					try {
						pCompare = std::any_cast<ICompare<std::string>::Ptr>(args[0]);
					}
					catch (std::bad_any_cast e) {
						throw std::invalid_argument("invalid argument type");
					}
					return std::any((ISort<std::string>::Ptr)std::make_shared<MergeSort<std::string>>(pCompare));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Quick"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 1)
						throw std::invalid_argument("invalid arguments number");
					ICompare<std::string>::Ptr pCompare;
					try {
						pCompare = std::any_cast<ICompare<std::string>::Ptr>(args[0]);
					}
					catch (std::bad_any_cast e) {
						throw std::invalid_argument("invalid argument type");
					}
					return std::any((ISort<std::string>::Ptr)std::make_shared<QuickSort<std::string>>(pCompare));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Selection"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 1)
						throw std::invalid_argument("invalid arguments number");
					ICompare<std::string>::Ptr pCompare;
					try {
						pCompare = std::any_cast<ICompare<std::string>::Ptr>(args[0]);
					}
					catch (std::bad_any_cast e) {
						throw std::invalid_argument("invalid argument type");
					}
					return std::any((ISort<std::string>::Ptr)std::make_shared<SelectionSort<std::string>>(pCompare));
				}
			}
		)->Do();
		Resolve<ICommand::Ptr>(
			"Default.Register",
			{
				std::string("Sort.Shell"),
				(Lambda)[](Fabric::Args args) {
					if (args.size() != 2)
						throw std::invalid_argument("invalid arguments number");
					ICompare<std::string>::Ptr pCompare;
					ISequence::Ptr pSequence;
					try {
						pCompare = std::any_cast<ICompare<std::string>::Ptr>(args[0]);
						pSequence = std::any_cast<ISequence::Ptr>(args[1]);
					}
					catch (std::bad_any_cast e) {
						throw std::invalid_argument("invalid argument type");
					}
					return std::any((ISort<std::string>::Ptr)std::make_shared<ShellSort<std::string>>(pCompare, pSequence));
				}
			}
		)->Do();
	}
};
