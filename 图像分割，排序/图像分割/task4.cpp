#include<opencv2/opencv.hpp>
using namespace cv;
Mat otsu1(int startx, int starty, int rows, int cols, Mat otsu);
void segment()
{
	Mat otsu = imread("task4.jpg", 0);
	int rows = otsu.rows / 2;
	int cols = otsu.cols / 3;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			otsu=otsu1(i*rows, j*cols, rows, cols, otsu);
		}
	}
	imshow("output", otsu);
	imwrite("output4.jpg", otsu);
	waitKey();
}
Mat otsu1(int startx,int starty, int rows,int cols,Mat otsu)
{
	//Mat otsu = imread("task1.jpg", 0);
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
		for (int i = startx; i < startx+rows; i++)
		{
			for (int j = starty; j < starty+cols; j++)
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
	for (int i = startx; i < startx+rows; i++)
	{
		for (int j = starty; j < starty+cols; j++)
		{
			if (otsu.at<uchar>(i, j) < threshhold)
				otsu.at<uchar>(i, j) = 255;
			else
			{
				otsu.at<uchar>(i, j) = 0;
			}
		}
	}
	return otsu;
}