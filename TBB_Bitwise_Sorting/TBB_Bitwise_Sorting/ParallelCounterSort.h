#pragma once
#include "Counter.h"
#include "tbb.h"
#include "task.h"

class ParallelCounterSort :public task {
private:   double *mas;  
		   double *tmp;  
		   int size;  
		   int nThreads; 
		   int *counters;  
		   int byteNum;

public:   ParallelCounterSort(double *_mas, double *_tmp, int _size, int _nThreads, int *_counters, int _byteNum) : mas(_mas), tmp(_tmp), size(_size), nThreads(_nThreads), counters(_counters), byteNum(_byteNum) {}

		  task* execute() {
			  Counter **ctr = new Counter*[nThreads - 1];   
			  Placer **pl = new Placer*[nThreads - 1];

			  int s = size / nThreads;

			  for (int i = 0; i<nThreads - 1; i++)   
				  ctr[i] = new (allocate_child()) 
				  Counter(mas + i*s, s, byteNum, counters + 256 * i);

			  Counter &ctrLast = *new (allocate_child())       
				  Counter(mas + s * (nThreads - 1), size - s * (nThreads - 1), byteNum, counters + 256 * (nThreads - 1));
			  set_ref_count(nThreads + 1);

			  for (int i = 0; i<nThreads - 1; i++)  
				  spawn(*(ctr[i]));

			  spawn_and_wait_for_all(ctrLast);

			  int sm = 0;

			  for (int j = 0; j<256; j++)
			  { 
				  for (int i = 0; i<nThreads; i++) 
				  { 
					  int b = counters[j + i * 256];     
					  counters[j + i * 256] = sm; 
					  sm += b; } }

			  for (int i = 0; i<nThreads - 1; i++)  
				  pl[i] = new (allocate_child()) Placer(mas + i*s, tmp, s, byteNum, counters + 256 * i);

			  Placer &plLast = *new (allocate_child()) 
				  Placer(mas + s * (nThreads - 1), tmp, size - s * (nThreads - 1), byteNum, counters + 256 * (nThreads - 1));

			  set_ref_count(nThreads + 1);

			  for (int i = 0; i<nThreads - 1; i++)   
				  spawn(*(pl[i]));

			  spawn_and_wait_for_all(plLast);

			  delete[] pl;     delete[] ctr;

			  return NULL;
		  }
};