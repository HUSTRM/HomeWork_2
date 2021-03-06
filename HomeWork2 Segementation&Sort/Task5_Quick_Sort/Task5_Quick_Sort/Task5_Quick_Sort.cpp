// Task5_Quick_Sort.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 快速排序(Quick Sort)是对冒泡排序的一种改进
// 基本思想是选取一个记录作为枢轴，经过一趟排序，将整段序列分为两个部分，其中一部分的值都小于枢轴，另一部分都大于枢轴
// 然后继续对这两部分继续进行排序，从而使整个序列达到有序

#include "pch.h"
#include <iostream>

using namespace std;

int GetMid(int *array, int left, int right); 
int PartitionSort(int *array, int left, int right); 
void QuickSort(int *array, int left, int right);

int main()
{
	int array[] = { 2,35,764,234,67,89,3245,34,46,976,2341 };
	QuickSort(array, 0, sizeof(array) / sizeof(array[0]) - 1);
	
	for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		cout << array[i] << ", ";
	}

	return 0;
}

// 三数取中法 ：
// 直接拿序列的最后一个值作为枢轴，如果最后这个值刚好是整段序列最大或者最小的值，那么这次划分就是没意义的
// 所以当序列是正序或者逆序时，每次选到的枢轴都是没有起到划分的作用，快排的效率会极速退化
// 所以可以每次在选枢轴时，在序列的第一，中间，最后三个值里面选一个中间值出来作为枢轴，保证每次划分接近均等
int GetMid(int *array, int left, int right)
{
	int mid = left + (right - left) / 2;
	if (array[left] <= array[right])
	{
		if (array[mid] < array[left])
			return left;
		else if (array[mid] > array[right])
			return right;
		else
			return mid;
	}
	else
	{
		if (array[mid] < array[right])
			return right;
		else if (array[mid] > array[left])
			return left;
		else
			return mid;
	}
}

// 左右指针法：
// 1.选取一个关键字(key)作为枢轴，一般取整组记录的第一个数 / 最后一个，这里采用选取序列最后一个数为枢轴。
// 2.设置两个变量left = 0; right = N - 1;
// 3.从left一直向后走，直到找到一个大于key的值，right从后至前，直至找到一个小于key的值，然后交换这两个数。
// 4.重复第三步，一直往后找，直到left和right相遇，这时将key放置left的位置即可。
/*

int PartitionSort(int *array, int left, int right)
{
	int mid = GetMid(array, left, right);
	swap(array[mid], array[right]);

	int& key = array[right];
	while (left < right)
	{
		while (left < right && array[left] <= key) // 因为有可能有相同的值，为了防止越界，所以加上left < right
		{
			++left;
		}
		while (left < right && array[right] >= key)
		{
			--right;
		}
		swap(array[left], array[right]);
	}
	swap(array[left], key);
	return left;
}
*/

// 挖坑法：
// 1.选取一个关键字(key)作为枢轴，一般取整组记录的第一个数 / 最后一个，这里采用选取序列最后一个数为枢轴，也是初始的坑位
// 2.设置两个变量left = 0， right = N - 1
// 3.从left一直向后走，直到找到一个大于key的值，然后将该数放入坑中，坑位变成了array[left]
// 4.right一直向前走，直到找到一个小于key的值，然后将该数放入坑中，坑位变成了array[right]
// 5.重复3和4的步骤，直到left和right相遇，然后将key放入最后一个坑位
int PartitionSort     (int*array, int left, int right)
{
	int mid = GetMid(array, left, right);
	swap(array[mid], array[right]);

	int key = array[right];
	while (left < right)
	{
		while (left < right && array[left] <= key)
		{
			++left;
		}
		array[right] = array[left];
		while (left < right && array[right] >= key)
		{
			--right;
		}
		array[left] = array[right];
	}
	array[right] = key;
	return right;
}


// 递归实现快速排序
void QuickSort(int *array, int left, int right)
{
	if (left >= right) // 表示已经完成一个组
		return;
	int index = PartitionSort(array, left, right); // 枢轴(pivot)的位置
	QuickSort(array, left, index - 1);
	QuickSort(array, index + 1, right);
}