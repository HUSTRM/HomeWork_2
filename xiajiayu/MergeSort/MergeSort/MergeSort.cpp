#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


void merge(vector<int>& nums, int l1, int r1, int l2, int r2) {
	int i = l1;                                               
	int j = l2;                                              
	int n = (r1 - l1 + 1) + (r2 - l2 + 1);                    
	vector<int> temp(n);                                      
	int k = 0;	                                          
	while (i <= r1 && j <= r2) {                                
		if (nums[i] < nums[j])
			temp[k++] = nums[i++];
		else
			temp[k++] = nums[j++];
	}

	while (i <= r1)
		temp[k++] = nums[i++];
	while (j <= r2)
		temp[k++] = nums[j++];
	//更新原始数组元素
	for (int i = 0; i < n;i++)
	{
		nums[l1 + i] = temp[i];
	}
}


void MergeSort(vector<int>& nums, int start, int end) {
	if (start < end) {
		int mid = (start + end) >> 1;				
		MergeSort(nums, start, mid);				
		MergeSort(nums, mid + 1, end);				
		merge(nums, start, mid, mid + 1, end);                  
	}
}


void MergeSort1(vector<int>& nums, int start, int end)
{
	int n = nums.size();
	if (start < end) {
		
		for (int step = 2; step / 2 < n; step *= 2) {
			for (int i = 0; i < n; i += step) {
				int mid = i + step / 2 - 1;					
				if (mid + 1 < n)							
					merge(nums, i, mid, mid + 1, min(i + step - 1, n - 1));
			}
		}
	}
}

int main() {
	vector<int> nums{ 1,4,3,2,5,6,3 };
	for (auto x : nums)
		cout << x << " ";
	cout << endl;
	MergeSort(nums, 0, 6);
	//	MergeSort1(nums, 0, 6);
	for (auto x : nums)
		cout << x << " ";
	cout << endl;
	system("pause");
	return 0;
}