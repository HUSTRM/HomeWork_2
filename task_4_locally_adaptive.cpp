#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
void locally_adaptive_threshold(Mat, int row_partition, int col_partition);
int otsu(Mat sample);
void thresholding(Mat, int threshold_value);
int main()
{
	Mat src = imread("2.tif", 0);
	locally_adaptive_threshold(src, 4, 6);
	imwrite("locally_adaptive_thresholding.tif", src);
	return 0;
}
void locally_adaptive_threshold(Mat src, int row_partition, int col_partition)
{
	int i, j;
	for(i=0;i<row_partition;i++)
		for (j = 0; j < col_partition; j++)
		{
			Mat sample = src(Rect(float(j / col_partition)*src.cols, float(i / row_partition)*src.rows, 
				float((j + 1) / col_partition)*src.cols, float((i+1) / row_partition)*src.rows));
			int threshold_value= otsu(sample);
			thresholding(sample, threshold_value);
		}
}
int otsu(Mat sample)
{
	int pixelcount[256];//��¼ÿ������ֵ��Ӧ�����ص����
	float pixelpro[256];//��¼ÿ������ֵ��Ӧ�����ص���ռ����
	int pixelsum = sample.rows*sample.cols;//���ص��ܸ���
	int i, j;
	for ( i = 0; i < 256; i++)
		pixelcount[i] = 0;
	for(i=0;i<sample.rows;i++)
		for (j = 0; j < sample.cols; j++)
			pixelcount[(int)sample.at<uchar>(i, j)]++;
	for (i = 0; i < 256; i++)
		pixelpro[i] = float(pixelcount[i]) / pixelsum;
	float gray_average1;//С�ڵ��ڵ�ǰ����ֵ���������ص������ֵ��ƽ��ֵ(��ռpixelsum�ı�������)
	float gray_average2;//���ڵ�ǰ����ֵ���������ص������ֵ��ƽ��ֵ(��ռpixelsum�ı�������)
	float pixelprosum1;//С�ڵ�ǰ����ֵ���������ص�ı��ʺ�
	float pixelprosum2;//���ڵ�ǰ����ֵ���������ص�ı��ʺ�
	float gray_average1_real;//С�ڵ��ڵ�ǰ����ֵ���������ص������ֵ��ƽ��ֵ
	float gray_average2_real; //���ڵ�ǰ����ֵ���������ص������ֵ��ƽ��ֵ
	float gray_average;//�������ص������ֵ��ƽ��ֵ
	float gray_variancemix;//��Ϸ���
	float gray_variancemax=0;//��󷽲�
	int threshold_value=0;
	for (i = 0; i < 256; i++)
	{
		gray_average1 = gray_average2 = gray_average =  pixelprosum1 = pixelprosum2 = gray_variancemix = 0;
		for (j = 0; j < 256; j++)
		{
			if (j <= i)
			{
				gray_average1 = pixelpro[j] * j;
				pixelprosum1 += pixelpro[j];
			}
			else
			{
				gray_average2 = pixelpro[j] * j;
				pixelprosum2 += pixelpro[j];
			}
		}
		gray_average1_real = gray_average1 / pixelprosum1;
		gray_average2_real = gray_average2 / pixelprosum1;
		gray_average = gray_average1 + gray_average2;
		gray_variancemix = pixelprosum1 * pow(gray_average1 - gray_average, 2) + pixelprosum2 * pow(gray_average2 - gray_average, 2);
		if (gray_variancemix > gray_variancemax)
		{
			gray_variancemax = gray_variancemix;
			threshold_value = i;
		}
	}
	return threshold_value;
}
void thresholding(Mat sample, int threshold_value)
{
	int i, j;
	for (i = 0; i < sample.rows; i++)
	{
		for (j = 0; j < sample.cols; j++)
		{
			if (sample.at<uchar>(i, j) <= threshold_value)
			{
				sample.at<uchar>(i, j) = 0;
			}
			else
			{
				sample.at<uchar>(i, j) = 255;
			}
		}
	}
}