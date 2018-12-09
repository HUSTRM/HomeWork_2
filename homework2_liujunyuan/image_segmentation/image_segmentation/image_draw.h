#pragma once
/*--------------------------------------------------------------------------------------------------------------------------------------
Copyright (C),2018---,   HUST Liu
 File name:
 Author: liu       Version: 1      Date:2018.12.7
 Description:
to draw image with gray bar
called by main_segmentation.cpp
-----------------------------------------------------------------------------------------------------------------------------------------
 Others:NONE
 May Function List: 
-----------------------------------------------------------------------------------------------------------------------------------------
 History  as folwing :
 NONE
  --------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------
-  openCV  standar head
---------------------------------------------------------------------------------------------------------------------------------------*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define max_gray_level 255
using namespace std;
using namespace cv;
/*-------------------------------------------------------------------------------------------------------------------
Function: mod
 Description:��������
 ---------------------------------------------------------------------------------------------------------------------
 Calls: NONE
 Called By: huizhi_zhifangtu
 Table Accessed: NONE
 Table Updated:  NONE
 --------------------------------------------------------------------------------------------------------------------
  Input:
			��һ������������a
			�ڶ�������������b
  Output��������
  Return:�����̣�int
  Others:NONE
--------------------------------------------------------------------------------------------------------------------*/
int mod(int value1, int value2)
{
	return (int)value1 / value2;
}
/*-------------------------------------------------------------------------------------------------------------------
Function: huizhi_zhifangtu
 Description:���ƻҶ�ͼ��ֱ��ͼ
 ---------------------------------------------------------------------------------------------------------------------
 Calls: mod
 Called By:zhifangtujunhenghua
 Table Accessed: NONE
 Table Updated:  NONE
 --------------------------------------------------------------------------------------------------------------------
  Input:
			��һ������������ͼ��Mat
			�ڶ�������������bin
  Output��չʾ������ֱ��ͼ
  Return:NONE
  Others:NONE
--------------------------------------------------------------------------------------------------------------------*/
void huizhi_zhifangtu(Mat input_image, int bin)
{
	double absolute_histo[max_gray_level+1] = { 0 };
	for (int i = 0; i < input_image.rows; i++)
	{
		const uchar* p = input_image.ptr<uchar>(i);
		for (int j = 0; j < input_image.cols; j++)
		{
			absolute_histo[mod(*(p + j), bin)]++;
		}
	}
	for (int flag = 0; flag < max_gray_level; flag++)
	{
		cout << absolute_histo[flag] << endl;
		cout << flag << endl;
	}
	Mat histImg;
	histImg.create(500, 800, CV_8UC1);
	histImg.setTo(0);
	for (int i = 0; i < max_gray_level+1; i++)
	{
		double tmpCount = absolute_histo[i] / (2 * bin);
		rectangle(histImg, Point2f(i * (2 * bin), 500), Point2f(i *(2 * bin), 500 - tmpCount), Scalar::all((int)max_gray_level / bin), -1);  //����ֱ��ͼ	
	}
	imshow("ֱ��ͼ", histImg);
	imwrite("ֱ��ͼ.png", histImg);//imwrite�ǵ�д��ʽ
}