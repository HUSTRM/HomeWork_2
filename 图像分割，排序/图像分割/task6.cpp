#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int otsuu();
void otsu3()
{
	int threshhold1 = otsuu();
	Mat otsu2 = imread("task6.jpg", 0);
	int threshhold2 = 0;
	unsigned int sum1 = 0, sum2 = 0;//前景与后景灰度分别的和
	int count1 = 0, count2 = 0;//前景与后景数
	double mean1 = 0, mean2 = 0;//前景后景平均数
	double max = 0;//方差最大值
	int i = 0;

	while (threshhold2++ < 256)
	{
		sum1 = 0;
		sum2 = 0;
		count1 = 0;
		count2 = 0;
		for (int i = 0; i < otsu2.rows; i++)
		{
			for (int j = 0; j < otsu2.cols; j++)
			{
				if (otsu2.at<uchar>(i, j) < threshhold2&&otsu2.at<uchar>(i, j) > threshhold1)
				{
					sum1 += otsu2.at<uchar>(i, j);
					count1++;
				}
				else if(otsu2.at<uchar>(i, j) > threshhold1)
				{
					sum2 += otsu2.at<uchar>(i, j);
					count2++;
				}
			}
		}
		mean1 = (double)sum1 / count1;
		mean2 = (double)sum2 / count2;
		double w0 = (double)count1 / (count1 + count2);
		double g = w0 * (1 - w0)*(mean1 - mean2)*(mean1 - mean2);
		if (max < g)
		{
			i = threshhold2;
			max = g;
		}
	}
	threshhold2 = i;
	cout << threshhold1 << "\n" << threshhold2 << endl;
	for (int i = 0; i < otsu2.rows; i++)
	{
		for (int j = 0; j < otsu2.cols; j++)
		{
			if (otsu2.at<uchar>(i, j) < threshhold1)
				otsu2.at<uchar>(i, j) = 255;
			else if (otsu2.at<uchar>(i, j) < threshhold2)
			{
				otsu2.at<uchar>(i, j) = 127;
			}
			else
			{
				otsu2.at<uchar>(i, j) = 0;
			}
		}
	}
	imshow("output", otsu2);
	imwrite("output6.jpg", otsu2);
	waitKey();
}
int otsuu()
{
	Mat otsu = imread("task6.jpg", 0);
	int threshhold = 0;
	unsigned int sum1 = 0, sum2 = 0;//前景与后景灰度分别的和
	int count1 = 0, count2 = 0;//前景与后景数
	double mean1 = 0, mean2 = 0;//前景后景平均数
	double max = 0;//方差最大值
	int i = 0;

	while (threshhold++ < 256)
	{
		sum1 = 0;
		sum2 = 0;
		count1 = 0;
		count2 = 0;
		for (int i = 0; i < otsu.rows; i++)
		{
			for (int j = 0; j < otsu.cols; j++)
			{
				if (otsu.at<uchar>(i, j) < threshhold)
				{
					sum1 += otsu.at<uchar>(i, j);
					count1++;
				}
				else
				{
					sum2 += otsu.at<uchar>(i, j);
					count2++;
				}
			}
		}
		mean1 = (double)sum1 / count1;
		mean2 = (double)sum2 / count2;
		double w0 = (double)count1 / (count1 + count2);
		double g = w0 * (1 - w0)*(mean1 - mean2)*(mean1 - mean2);
		if (max < g)
		{
			i = threshhold;
			max = g;
		}
	}
	threshhold = i;
	return threshhold;
}