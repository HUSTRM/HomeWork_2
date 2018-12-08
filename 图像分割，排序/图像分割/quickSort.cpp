#include<iostream>
using namespace std;
int mid(int A[], int left, int right)
{
	int mid = (right + left) / 2;
	//������Ԫ����������
	if (A[left] > A[mid])
		swap(A[left], A[mid]);
	if (A[left] > A[right])
		swap(A[left], A[right]);
	if (A[mid] > A[right])
		swap(A[mid], A[right]);
	//����ֵ�ŵ����һ��Ԫ��
	swap(A[mid], A[right - 1]);
	return A[right - 1];
}
void quickSort(int A[],int left,int right)
{
	int i, j;
	int pivot;
	
	if (left <= right - 3)//Ԫ�ظ���С�����޷��Ͳ�������ֵ�����п�������
	{
		pivot = mid(A, left, right);//��ֵȡ�з�
		i = left;
		j = right - 1;//ȡ�����ڶ���Ԫ�أ���ʼʱright=199������j=198,������һ��Ԫ�ش����ŦԪ
		while (true)
		{
			while (A[++i] < pivot) {}//��Ϊ��ŦԪ�����һ�������Ի᲻Խ��
			while (A[--j] > pivot) {}//��ΪA[left]�ڵ�һ��������Ҳ����Խ��
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
		//������Ԫ����������
		if (A[left] > A[mid])
			swap(A[left], A[mid]);
		if (A[left] > A[right])
			swap(A[left], A[right]);
		if (A[mid] > A[right])
			swap(A[mid], A[right]);
	}
	
}