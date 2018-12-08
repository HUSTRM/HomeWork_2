#include<iostream>
using namespace std;
void merge(int A[],int result[],int leftstart,int rightstart,int rightend)
{
	int i, leftend, numOfElement, tmp;
	leftend = rightstart - 1;
	tmp = leftstart;
	numOfElement = rightend - leftstart + 1;

	while (leftstart <= leftend && rightstart <= rightend)
		if (A[leftstart] <= A[rightstart])
			result[tmp++] = A[leftstart++];
		else
			result[tmp++] = A[rightstart++];
	while (leftstart <= leftend)
		result[tmp++] = A[leftstart++];
	while (rightstart <= rightend)
		result[tmp++] = A[rightstart++];
	for (int i = 0; i < numOfElement; i++, rightend--)
		A[rightend] = result[rightend];
}

void mergeSort(int A[], int result[], int left, int right)
{
	int mid;
	if (left < right)
	{
		mid = (left + right) / 2;
		mergeSort(A, result, left, mid);
		mergeSort(A, result, mid + 1, right);
		merge(A, result, left, mid + 1, right);
	}
}

void mergesort(int A[],int N)
{
	int *result;
	result = new int[N];
	if (result != NULL)
	{
		mergeSort(A, result, 0, N - 1);
		free(result);
	}
}