// Task1_Manual_Threshold.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat srcImage = imread("polymersomes.jpg", 0);
	int col = srcImage.cols;
	int row = srcImage.rows;
	int channel = srcImage.channels();
	// const uchar* image = srcImage.ptr();
	int threshold = 180; // 手动设定阈值
    
	cout << col << ' ' << row << ' ' << channel << endl;
	// cout << srcImage;

	// 遍历图像
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
