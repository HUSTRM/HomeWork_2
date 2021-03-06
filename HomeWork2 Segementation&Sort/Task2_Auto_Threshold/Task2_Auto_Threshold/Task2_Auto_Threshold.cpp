// Task2_Auto_Threshold.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat srcImage = imread("polymersomes.jpg", 0);
	//Mat srcImage = imread("girl.jpg", 0);
	int col = srcImage.cols;
	int row = srcImage.rows;
	
	double threshold = 160; // 估计全局阈值初值
	double bias = 0.1;
	double threshold_temp;

	bool flag = true;
	while (flag)
	{
		// 变量
		int sum1 = 0;
		int sum2 = 0;
		int count1 = 0;
		int count2 = 0;
		double mean1, mean2;
	
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if (srcImage.at<uchar>(i, j) < threshold)
				{
					sum1 += srcImage.at<uchar>(i, j);
					count1 += 1;
					mean1 = sum1 / count1;
				}
				else
				{
					sum2 += srcImage.at<uchar>(i, j);
					count2 += 1;
				    mean2 = sum2 / count2;
				}
			}
		}
		threshold_temp= (mean1 + mean2) / 2;

		if (fabs(threshold - threshold_temp) > bias)
		{
			threshold = threshold_temp;
			flag = false;
		}
	}

	cout << threshold;

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			// 阈值处理
			if (srcImage.at<uchar>(i, j) < threshold)
			{
				srcImage.at<uchar>(i, j) = 0;
			}
			else srcImage.at<uchar>(i, j) = 255;
		}
	}

	imshow("test", srcImage);
	imwrite("polymersomes_threshold.jpg", srcImage);

	waitKey(0);
	return 0;
}
