#pragma once

#include "../lesson.03.cpp/IExecuteable.h"

#include "ICompare.h"
#include "DefaultCompare.h"
#include "InverseCompare.h"
#include "AsNumberCompare.h"

#include "ISequence.h"
#include "BinarySequence.h"
#include "GonnetSequence.h"
#include "KnuthSequence.h"

// команда заполнения фабрики базовыми билдерами
class AppFabricSetupCommand : public IExecuteable
{
protected:
	Fabric::Ptr m_pScope;

public:
	AppFabricSetupCommand(Fabric::Ptr pScope) : m_pScope(std::move(pScope)) {}

	virtual void Execute() override {
		AddListOfCompare();
		AddListOfSequence();
		AddListOfSort();
	}

protected:

	void AddListOfCompare() {
		Fabric::Register(
			m_pScope,
			"Compare.Default",
			[](Fabric::Args args) {
				return std::any((ICompare<std::string>::Ptr)std::make_shared<DefaultCompare<std::string>>());
			}
		);
		Fabric::Register(
			m_pScope,
			"Compare.AsNumber",
			[](Fabric::Args args) {
				return std::any((ICompare<std::string>::Ptr)std::make_shared<AsNumberCompare>());
			}
		);
		Fabric::Register(
			m_pScope,
			"Compare.Inverse",
			[](Fabric::Args args) {
				ICompare<std::string>::Ptr pCompare = Fabric::GetRequiredArg<ICompare<std::string>::Ptr>(args, 0, "invalid base compare");
				return std::any((ICompare<std::string>::Ptr)std::make_shared<InverseCompare<std::string>>(pCompare));
			}
		);
	}
	void AddListOfSequence() {
		Fabric::Register(
			m_pScope,
			"Sequence.Binary",
			[](Fabric::Args args) {
				return std::any((ISequence::Ptr)std::make_shared<BinarySequence>());
			}
		);
		Fabric::Register(
			m_pScope,
			"Sequence.Gonnet",
			[](Fabric::Args args) {
				return std::any((ISequence::Ptr)std::make_shared<GonnetSequence>());
			}
		);
		Fabric::Register(
			m_pScope,
			"Sequence.Knuth",
			[](Fabric::Args args) {
				return std::any((ISequence::Ptr)std::make_shared<KnuthSequence>());
			}
		);
	}
	void AddListOfSort() {
		Fabric::Register(
			m_pScope,
			"Sort.Buble",
			[](Fabric::Args args) {
				ICompare<std::string>::Ptr pCompare = Fabric::GetRequiredArg<ICompare<std::string>::Ptr>(args, 0, "invalid item compare");
				return std::any((ISort<std::string>::Ptr)std::make_shared<BubbleSort<std::string>>(pCompare));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Heap",
			[](Fabric::Args args) {
				ICompare<std::string>::Ptr pCompare = Fabric::GetRequiredArg<ICompare<std::string>::Ptr>(args, 0, "invalid item compare");
				return std::any((ISort<std::string>::Ptr)std::make_shared<HeapSort<std::string>>(pCompare));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Insertion",
			[](Fabric::Args args) {
				ICompare<std::string>::Ptr pCompare = Fabric::GetRequiredArg<ICompare<std::string>::Ptr>(args, 0, "invalid item compare");
				return std::any((ISort<std::string>::Ptr)std::make_shared<InsertionSort<std::string>>(pCompare));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Merge",
			[](Fabric::Args args) {
				ICompare<std::string>::Ptr pCompare = Fabric::GetRequiredArg<ICompare<std::string>::Ptr>(args, 0, "invalid item compare");
				return std::any((ISort<std::string>::Ptr)std::make_shared<MergeSort<std::string>>(pCompare));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Quick",
			[](Fabric::Args args) {
				ICompare<std::string>::Ptr pCompare = Fabric::GetRequiredArg<ICompare<std::string>::Ptr>(args, 0, "invalid item compare");
				return std::any((ISort<std::string>::Ptr)std::make_shared<QuickSort<std::string>>(pCompare));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Selection",
			[](Fabric::Args args) {
				ICompare<std::string>::Ptr pCompare = Fabric::GetRequiredArg<ICompare<std::string>::Ptr>(args, 0, "invalid item compare");
				return std::any((ISort<std::string>::Ptr)std::make_shared<SelectionSort<std::string>>(pCompare));
			}
		);
		Fabric::Register(
			m_pScope,
			"Sort.Shell",
			[](Fabric::Args args) {
				ICompare<std::string>::Ptr pCompare = Fabric::GetRequiredArg<ICompare<std::string>::Ptr>(args, 0, "invalid item compare");
				ISequence::Ptr pSequence = Fabric::GetRequiredArg<ISequence::Ptr>(args, 1, "invalid item compare");
				return std::any((ISort<std::string>::Ptr)std::make_shared<ShellSort<std::string>>(pCompare, pSequence));
			}
		);
	}
};
