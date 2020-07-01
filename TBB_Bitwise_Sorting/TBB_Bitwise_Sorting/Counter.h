#pragma once
class Counter :public task {
	double *mas;   int size;   int byteNum;   int *counter;

public:   Counter(double *_mas, int _size, int _byteNum, int *_counter) : mas(_mas), size(_size), byteNum(_byteNum), counter(_counter) {}

		  task* execute() {
			  unsigned char *masUC = (unsigned char *)mas;

			  memset(counter, 0, sizeof(int) * 256);

			  for (int i = 0; i<size; i++)       counter[masUC[8 * i + byteNum]]++;

			  return NULL;
		  }
};