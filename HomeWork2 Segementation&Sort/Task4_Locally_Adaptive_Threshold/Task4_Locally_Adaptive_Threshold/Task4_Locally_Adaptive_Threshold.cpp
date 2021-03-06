// Task4_Locally_Adaptive_Threshold.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
Mat LocallyAdaptiveThreshold(const Mat image);

int main()
{
	Mat srcImage = imread("septagon_noisy_shaded.jpg", 0);
	
	srcImage = LocallyAdaptiveThreshold(srcImage);

	imshow("test", srcImage);
	imwrite("septagon_noisy_shaded_threshold.jpg", srcImage);

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

		uF = (double)SumF / NumF;
		uB = (double)SumB / NumB;
		wF = (double)NumF / NumTotal;
		wB = (double)NumB / NumTotal;

		between_class_variance_temp = wF * wB * (uF - uB) * (uF - uB); // 当前类间方差计算

		if (between_class_variance_temp > between_class_variance)
		{
			between_class_variance = between_class_variance_temp;
			Threshold = i;
		}
	}

	//cout << between_class_variance << endl;

	return Threshold;
}

Mat LocallyAdaptiveThreshold(const Mat image)
{
	int col = image.cols;
	int row = image.rows;
	int ROIcol = image.cols / 3;
    int ROIrow = image.rows / 2;
	Mat ROI;
	int Threshold;

	//Mat ROI[2][3];
	//ROI[0][0] = image(Rect(0, 0, ROIcol, ROIrow));
	//ROI[0][1] = image(Rect(ROIcol, 0, ROIcol, ROIrow));
	//ROI[0][2] = image(Rect(2 * ROIcol, 0, col - 2 * ROIcol, ROIrow));
	//ROI[1][0] = image(Rect(0, ROIrow, ROIcol, row - ROIrow));
	//ROI[1][1] = image(Rect(ROIcol, ROIrow, ROIcol, row - ROIrow));
	//ROI[1][2] = image(Rect(2 * ROIcol, ROIrow, col - 2 * ROIcol, row - ROIrow));

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// 划分子区域
			if (j == 0 || j == 1)
			{
			    ROI = image(Rect(j * ROIcol, i * ROIrow, ROIcol, abs(i * row - ROIrow)));
			}
			else
			{
				ROI = image(Rect(2 * ROIcol, i * ROIrow, col - 2 * ROIcol, abs(i * row - ROIrow)));
			}
			// 应用Otsu法
			Threshold = OTsuThreshold(ROI);
			for (int p = 0; p < ROI.rows; p++)
			{
				for (int q = 0; q < ROI.cols; q++)
				{
					if (ROI.at<uchar>(p, q) < Threshold)
					{
						ROI.at<uchar>(p, q) = 0;
					}
					else ROI.at<uchar>(p, q) = 255;
				}
			}
			//cout << Threshold << endl;
			//threshold(ROI, ROI, 0, 255, CV_THRESH_OTSU);
		}
	}
	return image;
}