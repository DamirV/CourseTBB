#include "tbb\parallel_sort.h"
#include "tbb\task_scheduler_init.h"
#include "LSDParallelSorter.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <time.h>

void Random(int *mas, int* mas2, int* copyMas, int size) {
	for (int i = 0; i < size; ++i) {
		mas[i] = mas2[i]= copyMas[i] = (rand()*rand())%2000000000;
	}
}

void Print(int* mas, int size) {
	for (int i = 0; i < size; ++i) {
		std::cout << mas[i] << " ";
	}
	std::cout << std::endl;
}

bool check(int* mas, int* copyMas, int size) {
	for (int i = 0; i < size; ++i) {
		if (mas[i] != copyMas[i]) {
			return false;
		}
	}
	return true;
}

int main() {
	srand(time(nullptr));
	int nThreads = 4;
	int size = 10000000;

	std::cout << "Number of threads: " << nThreads << std::endl;
	std::cout << "Size of array: " << size << std::endl;
	std::cout << std::endl;
	std::cout << "Memory allocation and writing arrays..." << std::endl;
	std::cout << std::endl;

	int* mas = new int[size];
	int* mas2 = new int[size];
	int* tempMas = new int[size];
	int* copyMas = new int[size];

	Random(mas, mas2, copyMas, size);
	double timeRes1;
	double timeRes2;
	if (size <= 10) {
		Print(mas, size);
		Print(copyMas, size);
	}
	
	std::cout << "Starting parallel sorting..." << std::endl;
	clock_t t1 = clock();
	tbb::task_scheduler_init init(nThreads);
	LSDParallelSorter& sorter1 = *new (tbb::task::allocate_root())
		LSDParallelSorter(mas, tempMas, size, nThreads);	tbb::task::spawn_root_and_wait(sorter1);	clock_t t2 = clock();
	timeRes1 = (t2 - t1 + .0) / CLOCKS_PER_SEC;
	

	init.terminate();
	init.initialize(1);

	std::cout << "Starting sorting with 1 stream..." << std::endl;
	t1 = clock();
	LSDParallelSorter& sorter2 = *new (tbb::task::allocate_root())
		LSDParallelSorter(mas2, tempMas, size, nThreads);	tbb::task::spawn_root_and_wait(sorter2);	 t2 = clock();

	 timeRes2 = (t2 - t1 + .0) / CLOCKS_PER_SEC;
	 double acceleration = timeRes2 / timeRes1;
	 double efficiency = acceleration / nThreads;

	 std::cout << std::endl;
	 std::cout << "Sorting time on " << nThreads << " streams: " << timeRes1 << std::endl;
	 std::cout << "Sorting time on 1 stream: " << timeRes2 << std::endl;
	 std::cout << "Acceleration: " << acceleration << std::endl;
	 std::cout << "Efficiency: " << efficiency << std::endl;
	 std::cout << std::endl;

	std::cout << "Validation check..." << std::endl;
	std::sort(copyMas, copyMas + size);

	if (size <= 20) {
		Print(mas, size);
		Print(copyMas, size);
	}

	if (check(mas, copyMas, size)) {
		std::cout << "Sorting is correct on "<<nThreads<< " streams" << std::endl;
	}
	else {
		std::cout << "Sorting is not correct on " << nThreads << " streams" << std::endl;
	}

	if (check(mas2, copyMas, size)) {
		std::cout << "Sorting is correct on 1 stream" << std::endl;
	}
	else {
		std::cout << "Sorting is not correct on 1 stream" << std::endl;
	}

	system("pause");
	return 0;
}