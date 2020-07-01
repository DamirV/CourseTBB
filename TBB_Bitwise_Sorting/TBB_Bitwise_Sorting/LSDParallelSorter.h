#pragma once
class LSDParallelSorter :public task
{
private:
	double *mas;
	double *tmp;
	int size;
	int nThreads;


public:   LSDParallelSorter(double *_mas, double *_tmp, int _size, int _nThreads) : mas(_mas), tmp(_tmp), size(_size), nThreads(_nThreads) {}

		  task* execute() {
			  int *counters = new int[256 * nThreads];

			  ParallelCounterSort *pcs = new (allocate_child())
				  ParallelCounterSort(mas, tmp, size, nThreads, counters, 0);
			  set_ref_count(2);
			  spawn_and_wait_for_all(*pcs);

			  pcs = new (allocate_child())
				  ParallelCounterSort(tmp, mas, size, nThreads, counters, 1);
			  set_ref_count(2);
			  spawn_and_wait_for_all(*pcs);

			  pcs = new (allocate_child())
				  ParallelCounterSort(mas, tmp, size, nThreads, counters, 2);
			  set_ref_count(2);
			  spawn_and_wait_for_all(*pcs);

			  pcs = new (allocate_child())
				  ParallelCounterSort(tmp, mas, size, nThreads, counters, 3);
			  set_ref_count(2);
			  spawn_and_wait_for_all(*pcs);

			  pcs = new (allocate_child())
				  ParallelCounterSort(mas, tmp, size, nThreads, counters, 4);
			  set_ref_count(2);
			  spawn_and_wait_for_all(*pcs);

			  pcs = new (allocate_child())
				  ParallelCounterSort(tmp, mas, size, nThreads,
					  set_ref_count(2);
			  spawn_and_wait_for_all(*pcs);

			  pcs = new (allocate_child())
				  ParallelCounterSort(mas, tmp, size, nThreads, counters, 6);
			  set_ref_count(2);
			  spawn_and_wait_for_all(*pcs);

			  pcs = new (allocate_child())
				  ParallelCounterSort(tmp, mas, size, nThreads, counters, 7);
			  set_ref_count(2);
			  spawn_and_wait_for_all(*pcs);
			  delete[] counters;

			  return NULL;
		  }
};