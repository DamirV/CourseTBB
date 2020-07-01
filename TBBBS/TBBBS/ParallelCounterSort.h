#pragma once
#include "Counter.h"
#include "Placer.h"

class ParallelCounterSort :public tbb::task
{
private:
	int *mas;
	int *tempMas;
	int size;
	int nThreads;
	int *counters;
	int byte;
public:
	ParallelCounterSort(int *_mas, int *_tempMas,
		int _size, int _nThreads,
		int *_counters, int _byteNum) :
		mas(_mas), tempMas(_tempMas), size(_size),
		nThreads(_nThreads),
		counters(_counters),
		byte(_byteNum)
	{}
	task* execute()
	{
		Counter **counter = new Counter*[nThreads - 1];
		Placer **placer = new Placer*[nThreads - 1];

		int s = size / nThreads;
		for (int i = 0; i < nThreads - 1; i++)
			counter[i] = new (allocate_child()) Counter(mas + i*s, s,
				byte, counters + 256 * i);

		Counter &counterLast = *new (allocate_child())
			Counter(mas + s * (nThreads - 1),
				size - s * (nThreads - 1),
				byte, counters + 256 * (nThreads - 1));
		set_ref_count(nThreads + 1);

		for (int i = 0; i<nThreads - 1; i++)
			spawn(*(counter[i]));
		spawn_and_wait_for_all(counterLast);
		int sm = 0;

		for (int j = 0; j<256; j++)
		{
			for (int i = 0; i<nThreads; i++)
			{
				int b = counters[j + i * 256];
				counters[j + i * 256] = sm;
				sm += b;
			}
		}

		for (int i = 0; i<nThreads - 1; i++)
			placer[i] = new (allocate_child()) Placer(mas + i*s, tempMas,
				s, byte, counters + 256 * i);

		Placer &placerLast = *new (allocate_child())
			Placer(mas + s * (nThreads - 1), tempMas,
				size - s * (nThreads - 1), byte,
				counters + 256 * (nThreads - 1));
		set_ref_count(nThreads + 1);

		for (int i = 0; i<nThreads - 1; i++)
			spawn(*(placer[i]));
		spawn_and_wait_for_all(placerLast);

		delete[] placer;
		delete[] counter;
		return NULL;
	}
};