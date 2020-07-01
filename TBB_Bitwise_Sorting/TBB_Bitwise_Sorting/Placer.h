#pragma once
#include "tbb.h"
#include "task.h"

class Placer :public task {
	double *inp, *out;   int size;   int byteNum;   int *counter;

public:   Placer(double *_inp, double *_out, int _size, int _byteNum, int *_counter) : inp(_inp), out(_out), size(_size), byteNum(_byteNum), counter(_counter) {}

		  task* execute()
		  {
			  unsigned char *inpUC = (unsigned char *)inp;

			  for (int i = 0; i<size; i++) { out[counter[inpUC[8 * i + byteNum]]] = inp[i];       counter[inpUC[8 * i + byteNum]]++; }

			  return NULL;
		  }
};