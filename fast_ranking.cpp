#include<iostream>
using namespace std;
int a[15] = { 17,45,2,99,84,53,22,66,100,38,74,110,45,93,77 };
void fast_ranking(int left, int right);
int main()
{
	fast_ranking(0, 14);
	for (int i = 0; i < 15; i++)
		cout << a[i] << "  ";
	cout << endl;
	system("pause");
	return 0;
}
void fast_ranking(int left, int right)
{
	if (left > right)
		return;
	int i, j, pivot, t;
	i = left; j = right;
	pivot = a[left];
	while (i!=j)
	{
		while (a[j] >= pivot&&i < j)
			j--;
		while (a[i] <= pivot&&i < j)
			i++;
		if (i < j)
		{
			t = a[i]; a[i] = a[j]; a[j] = t;
		}
	}
	a[left] = a[i];
	a[i] = pivot;
	fast_ranking(left, i - 1);
	fast_ranking(i + 1, right);
}
