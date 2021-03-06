// Task6_Merge_Sort.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

using namespace std;

void Merge(int* array, int left, int mid, int right);
void MergeSort(int* array, int left, int right);

int main()
{
	int array[] = { 233, 4353, 3244, 1143, 86, 435, 975, 234, 97867, 12435, 7856, 134};
	MergeSort(array, 0, sizeof(array) / sizeof(array[0]) - 1);
	for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		cout << array[i] << ", ";
	}
	return 0;
}

// 就地归并
void Merge(int* array, int left, int mid, int right) 
{
	int i = left, j = mid + 1;
	while (i <= mid && j <= right) 
	{
		if (array[i] > array[j]) 
		{
			// 向右平移左半部分
			int temp = array[j];
			for (int t = j; t > i; t--)
			{
				array[t] = array[t - 1];
			}
			array[i] = temp;
			++mid;
			++j;
		}
		++i;
	}
}

void MergeSort(int* array, int left, int right)
{
	if (left < right) 
	{
		int mid = (left + right) / 2;
		MergeSort(array, left, mid);
		MergeSort(array, mid + 1, right);
		Merge(array, left, mid, right);
	}
}

