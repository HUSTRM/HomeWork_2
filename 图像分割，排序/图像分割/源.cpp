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
	//manualThreshhold();						//����1���ֶ���ֵ
	//autoThreshhold();							//�������ȫ����ֵ
	//otsu();									//�����������
	//segment();								//�����ģ��ֲ����
	//segment2();								//����5
	//otsu3();									//����ֵ
	int a[200] = {};
	for (int i = 0; i < 200; i++)
	{
		a[i] = rand() % 1000;
	}
	//mergesort(a,200);							//�鲢����
	//quickSort(a, 0, 199);						//��������
	for (int i = 0; i < 200; i++)
	{
		std::cout << a[i] << "\t";
	}
	system("pause");
}