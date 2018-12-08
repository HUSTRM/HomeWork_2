#include<iostream>
using namespace std;
int mid(int A[], int left, int right)
{
	int mid = (right + left) / 2;
	//将三个元素升序排列
	if (A[left] > A[mid])
		swap(A[left], A[mid]);
	if (A[left] > A[right])
		swap(A[left], A[right]);
	if (A[mid] > A[right])
		swap(A[mid], A[right]);
	//将中值放到最后一个元素
	swap(A[mid], A[right - 1]);
	return A[right - 1];
}
void quickSort(int A[],int left,int right)
{
	int i, j;
	int pivot;
	
	if (left <= right - 3)//元素个数小于三无法就不能用中值法进行快速排序
	{
		pivot = mid(A, left, right);//三值取中法
		i = left;
		j = right - 1;//取倒数第二个元素，初始时right=199，所以j=198,倒数第一的元素存放枢纽元
		while (true)
		{
			while (A[++i] < pivot) {}//因为枢纽元在最后一个，所以会不越界
			while (A[--j] > pivot) {}//因为A[left]在第一个，所以也不会越界
			if (i < j)
				swap(A[i], A[j]);
			else break;
		}
		swap(A[i], A[right - 1]);
		quickSort(A, left, i - 1);
		quickSort(A, i + 1, right);
	}
	else
	{
		int mid = (right + left) / 2;
		//将三个元素升序排列
		if (A[left] > A[mid])
			swap(A[left], A[mid]);
		if (A[left] > A[right])
			swap(A[left], A[right]);
		if (A[mid] > A[right])
			swap(A[mid], A[right]);
	}
	
}