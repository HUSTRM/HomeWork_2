#include<opencv2/opencv.hpp>
void manualThreshhold();
void autoThreshhold();
void otsu();
void segment();
void segment2();
void otsu3();
void mergesort(int A[], int N);
void quickSort(int A[], int left, int right);
int main()
{
	//manualThreshhold();						//任务1，手动阈值
	//autoThreshhold();							//任务二，全局阈值
	//otsu();									//任务三，大津法
	//segment();								//任务四，局部大津法
	//segment2();								//任务5
	//otsu3();									//多阈值
	int a[200] = {};
	for (int i = 0; i < 200; i++)
	{
		a[i] = rand() % 1000;
	}
	//mergesort(a,200);							//归并排序
	//quickSort(a, 0, 199);						//快速排序
	for (int i = 0; i < 200; i++)
	{
		std::cout << a[i] << "\t";
	}
	system("pause");
}