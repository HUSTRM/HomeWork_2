#include "Sort.h"

void print(vector<unsigned> num) {
	for (int i = 1; i <= INT_RANDN; ++i) {
		printf("%10u  ", num[i - 1]);
		if (!(i % 7)) putchar('\n');
	}
}

vector<unsigned> RandGen(int n) {
	vector<unsigned> r;

	srand(static_cast<unsigned>(time(NULL)));
	for (int i = 0; i < n; ++i) {
		r.push_back(rand() % 25600);
	}
	return r;
}

void qsortMain(vector<unsigned> &num, int low, int high) {
	unsigned center = 0;
	if (low < high) {
		center = qsortPart(num, low, high);
		qsortMain(num, low, center - 1);
		qsortMain(num, center + 1, high);
	}
}

unsigned qsortPart(vector<unsigned> &num, int low, int high) {
	unsigned pivot = num[low];
	while (low < high) {
		while (low < high&&num[high] >= pivot)
			high--;
		if (low < high) num[low] = num[high];
		while (low < high&&num[low] <= pivot)
			low++;
		if (low < high) num[high] = num[low];
	}
	num[low] = pivot;
	return low;
}

void msortMain(vector<unsigned> &num, int from, int to, vector<unsigned> &res) {
	unsigned temp = 0;

	if (to - from == 1) {
		if (num[from] > num[to]) {
			SWAP(num[from], num[to], temp);
		}
		return;
	}
	else if (to - from == 0) return;
	else {
		msortMain(num, from, (to - from + 1) / 2 + from, res);
		msortMain(num, (to - from + 1) / 2 + from + 1, to, res);
		msortMerge(num, from, to, res);
		for (int i = from; i <= to; ++i) num[i] = res[i];
	}
}

void msortMerge(vector<unsigned> &num, int from, int to, vector<unsigned> &res) {
	int lenL = (to - from + 1) / 2 + 1;
	int idxL = from, idxR = from + lenL, idxRes = from;

	while (idxL < from + lenL && idxR <= to) {
		if (num[idxL] <= num[idxR]) res[idxRes++] = num[idxL++];
		else res[idxRes++] = num[idxR++];
	}
	while (idxL < from + lenL) res[idxRes++] = num[idxL++];
	while (idxR <= to) res[idxRes++] = num[idxR++];
}