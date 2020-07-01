#pragma once
#include "tbb\tbb.h"
#include "tbb\task.h"

class Placer :public tbb::task
{
	int *inp, *out;
	int size;
	int byteNum;
	int *counter;
public:
	Placer(int *_inp, int *_out, int _size,
		int _byteNum, int *_counter) : inp(_inp),
		out(_out), size(_size), byteNum(_byteNum),
		counter(_counter) {}

	task* execute()
	{
		unsigned char *inpUC = (unsigned char *)inp;
		for (int i = 0; i<size; i++)
		{
			out[counter[inpUC[4 * i + byteNum]]] = inp[i];
			counter[inpUC[4 * i + byteNum]]++;
		}
		return NULL;
	}
};