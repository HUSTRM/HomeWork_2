// Task3_Otsu_Threshold.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int OTsuThreshold(const Mat image);

int main()
{
	Mat srcImage = imread("polymersomes.jpg", 0);
	int col = srcImage.cols;
	int row = srcImage.rows;
	
	int Threshold = OTsuThreshold(srcImage); 
	cout << Threshold;

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			// 阈值处理
			if (srcImage.at<uchar>(i, j) < Threshold)
			{
				srcImage.at<uchar>(i, j) = 0;
			}
			else srcImage.at<uchar>(i, j) = 255;
		}
	}

	//Mat imageOtsu;
	//threshold(srcImage, imageOtsu, 0, 255, CV_THRESH_OTSU);

	imshow("test", srcImage);
	//imshow("otsu", imageOtsu);
	//imwrite("polymersomes_threshold.jpg", srcImage);

	waitKey(0);
	return 0;
}

int OTsuThreshold(const Mat image)
{
	int Threshold = 0;
	double wF = 0; // 前景比例
	double wB = 0; // 背景比例
	int NumF = 0; // 前景总数
	int NumB = 0; // 背景总数
	int SumF = 0; // 前景灰度总值
	int SumB = 0; // 背景灰度总值
	double uF = 0; // 前景平均灰度
	double uB = 0; // 背景平均灰度
	double between_class_variance_temp;
	double between_class_variance = 0;
	int Histogram[256] = { 0 };
	uchar *data = image.data;
	int NumTotal = image.cols * image.rows;

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			Histogram[data[i * image.step + j]]++;
		}
	}

	/*
	for (int i = 0; i < 256; i++)
	{
		cout << i << ':' << Histogram[i] << endl;
	}
	*/

	for (int i = 0; i < 255; i++)     // i作为阈值
	{
		// 初始化
		wF = wB = uF = uB = between_class_variance_temp = .0;
		NumF = NumB = SumF = SumB = 0;

		for (int j = 0; j < 255; j++)
		{
			if (j < i)   //背景部分  
			{
				NumB += Histogram[j];
				SumB += j * Histogram[j];
			}
			else   //前景部分  
			{
				NumF += Histogram[j];
				SumF += j * Histogram[j];
			}
		}
		
		uF = (double) SumF / NumF;
		uB = (double) SumB / NumB;
		wF = (double) NumF / NumTotal;
		wB = (double) NumB / NumTotal;

		between_class_variance_temp = wF * wB * (uF - uB) * (uF - uB); // 当前类间方差计算
		
		if (between_class_variance_temp > between_class_variance)
		{
			between_class_variance = between_class_variance_temp;
			Threshold = i;
		}
	}

	cout << between_class_variance << endl;

	return Threshold;
}