/*--------------------------------------------------------------------------------------------------------------------------------------
Copyright (C),2018---,   HUST Liu
 File name:main_segmentation.cpp
 Author: liu       Version: 1      Date:2018.12.9
 Description: 
to segmentate some image
But The effect is not good
-----------------------------------------------------------------------------------------------------------------------------------------
 Others:NONE
 May Function List: 
 threshold_segmentation
threshold_segmentation_auto
globel_otsu_segmentation_auto
windows_otsu_segmentation_auto
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
#include "image_draw.h"
#include "segmfunction.h"
#define max_gray_level 255
using namespace std;
using namespace cv;
int main()
{
	Mat image1=imread("task1.png",0);
	Mat image3= imread("task23.png", 0);
	Mat image4 = imread("task4.png", 0);
	Mat image2 = imread("task23.png", 0);
	Mat image5 = imread("task4.png", 0);
	if (!image1.data)
	{
		cout << "reading image wrong!!";
	}
	if (!image2.data)
	{
		cout << "reading image wrong!!";
	}
	if (!image3.data)
	{
		cout << "reading image wrong!!";
	}
	if (!image4.data)
	{
		cout << "reading image wrong!!";
	}
	threshold_segmentation(image1, image1, 91, 160);
	threshold_segmentation_auto(image2, image2, 90, 0.01, 1000);
	globel_otsu_segmentation_auto(image3, image3);
	windows_otsu_segmentation_auto(image4, image5, 10);
	imshow("1", image1);
	imshow("2", image2);
	imshow("3", image3);
	imshow("4", image5);
	waitKey(0);
}