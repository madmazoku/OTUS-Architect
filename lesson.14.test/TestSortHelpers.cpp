#include "pch.h"

#include "TestSortHelpers.h"

void Helper::TestSort(std::vector<int> items, ISort<int>::Ptr pSort) {
	pSort->Sort(items);
	for (int idx = 0; idx < items.size() - 1; ++idx)
		EXPECT_TRUE(items[idx] <= items[idx + 1]);
}

std::vector<int> Helper::RandomItems() {
	std::random_device rd;
	std::default_random_engine re(rd());
	std::uniform_int_distribution<int> ud_N(10, 1000);
	std::uniform_int_distribution<int> ud_Item(0, 100);

	std::vector<int> items(ud_N(re));
	for (size_t idx = 0; idx < items.size(); ++idx)
		items[idx] = ud_Item(re);
	return items;
}

std::vector<int> Helper::AscendingItems() {
	std::random_device rd;
	std::default_random_engine re(rd());
	std::uniform_int_distribution<int> ud_N(10, 1000);
	std::uniform_int_distribution<int> ud_Item(0, 100);

	std::vector<int> items(ud_N(re));
	int value = ud_Item(re);
	for (size_t idx = 0; idx < items.size(); ++idx)
		items[idx] = (value += ud_Item(re));
	return items;
}

std::vector<int> Helper::DescendingItems() {
	std::random_device rd;
	std::default_random_engine re(rd());
	std::uniform_int_distribution<int> ud_N(10, 1000);
	std::uniform_int_distribution<int> ud_Item(0, 100);

	std::vector<int> items(ud_N(re));
	int value = ud_Item(re);
	for (size_t idx = items.size(); idx > 0; --idx)
		items[idx - 1] = (value += ud_Item(re));
	return items;
}