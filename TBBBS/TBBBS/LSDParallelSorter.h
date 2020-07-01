#pragma once
#include "ParallelCounterSort.h"

class LSDParallelSorter :public tbb::task
{
private:
	int *mas;
	int *tempMas;
	int size;
	int nThreads;
public:
	LSDParallelSorter(int *_mas, int *_tempMas, int _size,
		int _nThreads) : mas(_mas), tempMas(_tempMas),
		size(_size), nThreads(_nThreads)
	{}
	task* execute()
	{
		int *counters = new int[256 * nThreads];
		ParallelCounterSort *pcs = new (allocate_child())
			ParallelCounterSort(mas, tempMas, size, nThreads,
				counters, 0);
		set_ref_count(2);
		spawn_and_wait_for_all(*pcs);

		pcs = new (allocate_child())
			ParallelCounterSort(tempMas, mas, size, nThreads,
				counters, 1);
		set_ref_count(2);
		spawn_and_wait_for_all(*pcs);

		pcs = new (allocate_child())
			ParallelCounterSort(mas, tempMas, size, nThreads,
				counters, 2);
		set_ref_count(2);
		spawn_and_wait_for_all(*pcs);

		pcs = new (allocate_child())
			ParallelCounterSort(tempMas, mas, size, nThreads,
				counters, 3);
		set_ref_count(2);
		spawn_and_wait_for_all(*pcs);

		delete[] counters;
		return NULL;
	}
};