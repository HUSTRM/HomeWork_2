#include<iostream>
using namespace std;
void ranking(int *p, int n);
int * merge_ranking(int *p, int n1, int *q, int n2);
int main()
{
	int a[10] = { 49,73,6,80,92,57,77,32,40,100 };
	int b[9] = { 95,19,31,89,70,44,37,22,99 };
	ranking(a, 10);
	ranking(b, 9);
	int *n=merge_ranking(a, 10, b, 9);
	for (int i = 0; i < 19; i++, n++)
		cout << *n << "  ";
	cout << endl;
	system("pause");
	return 0;
}
/*void ranking(int *p, int n)
{
	int i, j, t;
	for (i = 0; i < n - 1; i++,p++)
	{
		int *q = p + 1;
		for (j = i+1; j < n; j++,q++)
		{
			if (*q < *p)
			{
				t = *q; *q = *p; *p = t;
			}
		}
	}
}*/
void ranking(int *p, int n)
{
	int i, j, t;
	for (i = 0; i < n - 1; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (p[j] < p[i])
			{
				t = p[j]; p[j] = p[i]; p[i] = t;
			}
		}
	}
}
int *merge_ranking(int *p, int n1, int *q, int n2)
{
	int *m = new int[n1 + n2];
	int i = 0, j = 0, s = 0;
	while (i != n1 && j != n2)
	{
		if (*p < *q)
		{
			m[s] = *p; p++; s++; i++;
		}
		else
		{
			m[s] = *q; q++; s++; j++;
		}
	}
	if (i == n1)
	{
		while (s != n1 + n2)
		{
			m[s] = *q; q++; s++;
		}
	}
	else
	{
		while (s != n1 + n2)
		{
			m[s] = *p; p++; s++;
		}
	}
	return m;
}