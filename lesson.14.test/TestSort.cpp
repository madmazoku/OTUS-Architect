#include "pch.h"

#include <string>

#include "TestSortHelpers.h"

#include "../lesson.14.cpp/ICompare.h"
#include "../lesson.14.cpp/AsNumberCompare.h"
#include "../lesson.14.cpp/DefaultCompare.h"
#include "../lesson.14.cpp/InverseCompare.h"

#include "../lesson.14.cpp/ISequence.h"
#include "../lesson.14.cpp/BinarySequence.h"
#include "../lesson.14.cpp/GonnetSequence.h"
#include "../lesson.14.cpp/KnuthSequence.h"

TEST(BubbleSort, RandomOrder) {
	Helper::TestSort(Helper::RandomItems(), std::make_shared<BubbleSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(BubbleSort, AscendingOrder) {
	Helper::TestSort(Helper::AscendingItems(), std::make_shared<BubbleSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(BubbleSort, DescendingOrder) {
	Helper::TestSort(Helper::DescendingItems(), std::make_shared<BubbleSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(HeapSort, RandomOrder) {
	Helper::TestSort(Helper::RandomItems(), std::make_shared<HeapSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(HeapSort, AscendingOrder) {
	Helper::TestSort(Helper::AscendingItems(), std::make_shared<HeapSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(HeapSort, DescendingOrder) {
	Helper::TestSort(Helper::DescendingItems(), std::make_shared<HeapSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(InsertionSort, RandomOrder) {
	Helper::TestSort(Helper::RandomItems(), std::make_shared<InsertionSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(InsertionSort, AscendingOrder) {
	Helper::TestSort(Helper::AscendingItems(), std::make_shared<InsertionSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(InsertionSort, DescendingOrder) {
	Helper::TestSort(Helper::DescendingItems(), std::make_shared<InsertionSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(MergeSort, RandomOrder) {
	Helper::TestSort(Helper::RandomItems(), std::make_shared<MergeSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(MergeSort, AscendingOrder) {
	Helper::TestSort(Helper::AscendingItems(), std::make_shared<MergeSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(MergeSort, DescendingOrder) {
	Helper::TestSort(Helper::DescendingItems(), std::make_shared<MergeSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(QuickSort, RandomOrder) {
	Helper::TestSort(Helper::RandomItems(), std::make_shared<QuickSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(QuickSort, AscendingOrder) {
	Helper::TestSort(Helper::AscendingItems(), std::make_shared<QuickSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(QuickSort, DescendingOrder) {
	Helper::TestSort(Helper::DescendingItems(), std::make_shared<QuickSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(SelectionSort, RandomOrder) {
	Helper::TestSort(Helper::RandomItems(), std::make_shared<SelectionSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(SelectionSort, AscendingOrder) {
	Helper::TestSort(Helper::AscendingItems(), std::make_shared<SelectionSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(SelectionSort, Descendingrder) {
	Helper::TestSort(Helper::DescendingItems(), std::make_shared<SelectionSort<int>>(std::make_shared<DefaultCompare<int>>()));
}

TEST(ShellSort, RandomOrder) {
	Helper::TestSort(Helper::RandomItems(), std::make_shared<ShellSort<int>>(std::make_shared<DefaultCompare<int>>(), std::make_shared<KnuthSequence>()));
}

TEST(ShellSort, AscendingOrder) {
	Helper::TestSort(Helper::AscendingItems(), std::make_shared<ShellSort<int>>(std::make_shared<DefaultCompare<int>>(), std::make_shared<KnuthSequence>()));
}

TEST(ShellSort, DescendingOrder) {
	Helper::TestSort(Helper::DescendingItems(), std::make_shared<ShellSort<int>>(std::make_shared<DefaultCompare<int>>(), std::make_shared<KnuthSequence>()));
}

TEST(DefaultCompare, Int) {
	ICompare<int>::Ptr pCompare = std::make_shared<DefaultCompare<int>>();
	EXPECT_EQ(pCompare->Compare(1, 1), 0);
	EXPECT_EQ(pCompare->Compare(1, 2), -1);
	EXPECT_EQ(pCompare->Compare(2, 1), 1);
}

TEST(DefaultCompare, String) {
	ICompare<std::string>::Ptr pCompare = std::make_shared<DefaultCompare<std::string>>();
	EXPECT_EQ(pCompare->Compare("1", "1"), 0);
	EXPECT_EQ(pCompare->Compare("11", "2"), -1);
	EXPECT_EQ(pCompare->Compare("2", "11"), 1);
}

TEST(AsNumberCompare, String) {
	ICompare<std::string>::Ptr pCompare = std::make_shared<AsNumberCompare>();
	EXPECT_EQ(pCompare->Compare("1", "1"), 0);
	EXPECT_EQ(pCompare->Compare("2", "11"), -1);
	EXPECT_EQ(pCompare->Compare("11", "2"), 1);
}

TEST(InverseCompare, Int) {
	ICompare<int>::Ptr pDefaultCompare = std::make_shared<DefaultCompare<int>>();
	ICompare<int>::Ptr pCompare = std::make_shared<InverseCompare<int>>(pDefaultCompare);
	EXPECT_EQ(pCompare->Compare(1, 1), 0);
	EXPECT_EQ(pCompare->Compare(2, 1), -1);
	EXPECT_EQ(pCompare->Compare(1, 2), 1);
}

TEST(BinarySequence, Initialize) {
	ISequence::Ptr pSequence = std::make_shared<BinarySequence>();
	EXPECT_EQ(pSequence->Initialize(100), 50);
}

TEST(BinarySequence, Decrease) {
	ISequence::Ptr pSequence = std::make_shared<BinarySequence>();
	EXPECT_EQ(pSequence->Decrease(100), 50);
	EXPECT_EQ(pSequence->Decrease(1), 0);
}

TEST(GonnetSequence, Initialize) {
	ISequence::Ptr pSequence = std::make_shared<GonnetSequence>();
	EXPECT_EQ(pSequence->Initialize(100), 45);
}

TEST(GonnetSequence, Decrease) {
	ISequence::Ptr pSequence = std::make_shared<GonnetSequence>();
	EXPECT_EQ(pSequence->Decrease(100), 45);
	EXPECT_EQ(pSequence->Decrease(2), 1);
	EXPECT_EQ(pSequence->Decrease(1), 0);
}

TEST(KnuthSequence, Initialize) {
	ISequence::Ptr pSequence = std::make_shared<KnuthSequence>();
	EXPECT_EQ(pSequence->Initialize(100), 40);
}

TEST(KnuthSequence, Decrease) {
	ISequence::Ptr pSequence = std::make_shared<KnuthSequence>();
	EXPECT_EQ(pSequence->Decrease(100), 67);
	EXPECT_EQ(pSequence->Decrease(3), 1);
	EXPECT_EQ(pSequence->Decrease(2), 1);
	EXPECT_EQ(pSequence->Decrease(1), 0);
}