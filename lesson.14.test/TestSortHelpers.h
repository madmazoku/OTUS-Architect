#pragma once

#include <random>
#include <vector>
#include <memory>

#include "../lesson.14.cpp/ISort.h"
#include "../lesson.14.cpp/BubbleSort.h"
#include "../lesson.14.cpp/HeapSort.h"
#include "../lesson.14.cpp/InsertionSort.h"
#include "../lesson.14.cpp/MergeSort.h"
#include "../lesson.14.cpp/QuickSort.h"
#include "../lesson.14.cpp/SelectionSort.h"
#include "../lesson.14.cpp/ShellSort.h"

namespace Helper {
	std::vector<int> RandomItems();
	std::vector<int> AscendingItems();
	std::vector<int> DescendingItems();
	void TestSort(std::vector<int> items, ISort<int>::Ptr pSort);
};
