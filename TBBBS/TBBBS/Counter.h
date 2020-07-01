#pragma once
#include "tbb\tbb.h"
#include "tbb\task.h"

class Counter :public tbb::task
{
	int *mas;
	int size;
	int byteNum;
	int *counter;
public:
	Counter(int *_mas, int _size, int _byteNum,
		int *_counter) : mas(_mas), size(_size),
		byteNum(_byteNum), counter(_counter) {}

	task* execute()
	{
		unsigned char *masUC = (unsigned char *)mas;
		memset(counter, 0, sizeof(int) * 256);
		for (int i = 0; i<size; i++)
			counter[masUC[4 * i + byteNum]]++;
		return NULL;
	}
};