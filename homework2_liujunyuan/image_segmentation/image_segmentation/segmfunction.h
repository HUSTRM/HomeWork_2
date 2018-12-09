#pragma once
/*--------------------------------------------------------------------------------------------------------------------------------------
Copyright (C),2018---,   HUST Liu
 File name:
 Author: liu       Version: 1      Date:2018.12.9
 Description:

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
#include "image_draw.h"
#include "sort_function.h"
#include <math.h>
#define max_gray_level 255
#define P_all 1.0
using namespace std;
using namespace cv;

/*-------------------------------------------------------------------------------------------------------------------
Function: threshold_segmentation
 Description:threshold segmentation with 2 paramas threshold
 ---------------------------------------------------------------------------------------------------------------------
 Calls: NONE
 Called By: 
 Table Accessed: NONE
 Table Updated:  NONE
 --------------------------------------------------------------------------------------------------------------------
  Input:
@Mat imput_image:mat image you want to segmentate(hui du tu)
@Mat output_image:output Mat image
@param1:lowest threshold
@param2:the second threshold
  Output：output Mat image had been segmentated
  Return:NONE
  Others:NONE
--------------------------------------------------------------------------------------------------------------------*/
void threshold_segmentation(Mat input_image,Mat &output_image,int param1,int param2)
{
	output_image.create(input_image.rows, output_image.cols, input_image.type());
	for (int flag_rows = 0; flag_rows < input_image.rows; flag_rows++)
	{
		for (int flag_cols = 0; flag_cols < input_image.cols; flag_cols++)
		{
			if (input_image.at<uchar>(flag_rows, flag_cols) <= param1)
			{
				output_image.at<uchar>(flag_rows, flag_cols) = 0;
			}
			else if ((input_image.at<uchar>(flag_rows, flag_cols) <= param2) && (input_image.at<uchar>(flag_rows, flag_cols) > param1))
			{
				output_image.at<uchar>(flag_rows, flag_cols) = 150;
			}
			else
			{
				output_image.at<uchar>(flag_rows, flag_cols) = max_gray_level;
			}
		}
	}
}


/*-------------------------------------------------------------------------------------------------------------------
Function: threshold_segmentation_auto
 Description:threshold segmentation with an initial paramas threshold autoly
 ---------------------------------------------------------------------------------------------------------------------
 Calls: NONE
 Called By:main_segmentation.cpp
 Table Accessed: NONE
 Table Updated:  NONE
 --------------------------------------------------------------------------------------------------------------------
  Input:
@Mat imput_image:mat image you want to segmentate(hui du tu)
@Mat output_image:output Mat image
@int initial_threshold:initial threshold
@float delta:the min bias
@int max_times:the max times your computer calculate
  Output：output Mat image had been segmentated
  Return:the threshold 
  Others:NONE
--------------------------------------------------------------------------------------------------------------------*/
float threshold_segmentation_auto(Mat input_image,Mat &output_image,int initial_threshold,float delta,int max_times)
{
	output_image.create(input_image.rows, output_image.cols, input_image.type());
	int absolute_histo1[max_gray_level+1] = { 0 };
	int absolute_histo2[max_gray_level+1] = { 0 };
	float sum[2] = { 0 };
	float all[2] = { 0 };
	float mean[4] = { 0 };
	int real_times = 0;
	mean[3] = initial_threshold;
	while(1)
	{
		for (int flag_rows = 0; flag_rows < input_image.rows; flag_rows++)
		{
			for (int flag_cols = 0; flag_cols < input_image.cols; flag_cols++)
			{
				if (input_image.at<uchar>(flag_rows, flag_cols) < mean[3])
				{
					int like_hood = (int)input_image.at<uchar>(flag_rows, flag_cols);
					absolute_histo1[like_hood]++;
				}
				else
				{
					int like_hood = (int)input_image.at<uchar>(flag_rows, flag_cols);
					absolute_histo2[like_hood]++;
				}
			}
		}
		for (int flag = 0; flag <= max_gray_level; flag++)
		{
			sum[0] = sum[0] + absolute_histo1[flag];
			sum[1] = sum[1] + absolute_histo2[flag];
			all[0] = all[0] + flag * absolute_histo1[flag];
			all[1] = all[1] + flag * absolute_histo1[flag];
		}
		mean[0] = all[0] / sum[0];
		mean[1] = all[1] / sum[0];
		mean[2] = (mean[0] + mean[1]) / 2;
		if (fabs(mean[2] - mean[3]) < delta || real_times >= max_times)
		{
			break;
		}
		mean[3] = mean[2];
		sum[0] = 0;
		sum[1] = 0;
		all[0] = 0;
		all[1] = 0;
		real_times++;
	}
	for (int flag_rows = 0; flag_rows < input_image.rows; flag_rows++)
	{
		for (int flag_cols = 0; flag_cols < input_image.cols; flag_cols++)
		{
			if (input_image.at<uchar>(flag_rows, flag_cols) <= mean[2])
			{
				output_image.at<uchar>(flag_rows, flag_cols) = 0;
			}
			else
			{
				output_image.at<uchar>(flag_rows, flag_cols) = max_gray_level;
			}
		}
	}
	return real_times;
}


/*-------------------------------------------------------------------------------------------------------------------
Function: adapt_function_in_otsu
 Description:adapt_function for otsu_segmentation_auto
 ---------------------------------------------------------------------------------------------------------------------
 Calls: NONE
 Called By: otsu_segmentation_auto
 Table Accessed: NONE
 Table Updated:  NONE
 --------------------------------------------------------------------------------------------------------------------
  Input:
@float k:x
@float absolute_histo[]:array of histogram
  Output：NONE
  Return:sigmaB
  Others:NONE
--------------------------------------------------------------------------------------------------------------------*/
float adapt_function_in_otsu(int k,float absolute_histo[])
{
	float P1k = 0.;
	float Mk = 0.;
	float Mg = 0.;
	for (int flag = 0; flag <= k; flag++)
	{
		P1k += absolute_histo[flag];
	}
	for (int flag = 0; flag <= k; flag++)
	{
		Mk += absolute_histo[flag] * flag;
	}
	for (int flag = 0; flag <= max_gray_level; flag++)
	{
		Mg += absolute_histo[flag] * flag;
	}
	if((P1k!=1)&&(P1k!=0))
	{
		return ((Mg*P1k - Mk)*(Mg*P1k - Mk)) / ((P1k)*(P_all - P1k));
	}
	else
	{ 
		return 0; 
	}
}

/*-------------------------------------------------------------------------------------------------------------------
Function: globel_otsu_segmentation_auto
 Description:threshold segmentation with otsu method autoly
 ---------------------------------------------------------------------------------------------------------------------
 Calls: NONE
 Called By:main_segmentation.cpp  windows_otsu_segmentation_auto
 Table Accessed: NONE
 Table Updated:  NONE
 --------------------------------------------------------------------------------------------------------------------
  Input:
@Mat imput_image:mat image you want to segmentate(hui du tu)
@Mat output_image:output Mat image
  Output：output Mat image had been segmentated
  Return:the threshold
  Others:NONE
--------------------------------------------------------------------------------------------------------------------*/
float globel_otsu_segmentation_auto(Mat input_image, Mat &output_image)
{
	output_image.create(input_image.rows, output_image.cols, input_image.type());
	float absolute_histo[max_gray_level+1] = { 0.};
	double sigmaB[max_gray_level+1] = { 0. };
	float NM =0 ;
	float threshold=0;
	NM = input_image.rows*input_image.cols;
	for (int flag_rows = 0; flag_rows < input_image.rows; flag_rows++)
	{
		for (int flag_cols = 0; flag_cols < input_image.cols; flag_cols++)
		{
				int like_hood = (int)input_image.at<uchar>(flag_rows, flag_cols);
				absolute_histo[like_hood]++;
		}
	}	
	for (int flag = 0; flag <= max_gray_level; flag++)
	{
		absolute_histo[flag] = absolute_histo[flag] / NM;
	}
	for (int flag = 0; flag <= max_gray_level; flag++)
	{
		sigmaB[flag] = (double)adapt_function_in_otsu(flag, absolute_histo);
	}
	threshold = my_max(sigmaB);
	for (int flag_rows = 0; flag_rows < input_image.rows; flag_rows++)
	{
		for (int flag_cols = 0; flag_cols < input_image.cols; flag_cols++)
		{
			if (input_image.at<uchar>(flag_rows, flag_cols) <= threshold)
			{
				output_image.at<uchar>(flag_rows, flag_cols) = 0;
			}
			else
			{
				output_image.at<uchar>(flag_rows, flag_cols) = max_gray_level;
			}
		}
	}
	return threshold;
}


/*-------------------------------------------------------------------------------------------------------------------
Function: mean_auto
 Description:get gray mean
 ---------------------------------------------------------------------------------------------------------------------
 Calls: NONE
 Called By:var_auto
 Table Accessed: NONE
 Table Updated:  NONE
 --------------------------------------------------------------------------------------------------------------------
  Input:
@Mat imput_image:mat image you want to mean

  Output：NONE
  Return:mean gray 
  Others:NONE
--------------------------------------------------------------------------------------------------------------------*/
float mean_auto(Mat input_image)
{
	float absolute_histo[max_gray_level + 1] = { 0. };
	float sum=0.;
	float NM = input_image.cols*input_image.rows;
	//获取灰度直方图
	for (int flag_rows = 0; flag_rows < input_image.rows; flag_rows++)
	{
		for (int flag_cols = 0; flag_cols < input_image.cols; flag_cols++)
		{
			int like_hood = (int)input_image.at<uchar>(flag_rows, flag_cols);
			absolute_histo[like_hood]++;
		}
	}
	for (int flag = 0; flag <= max_gray_level; flag++)
	{
		sum += flag*absolute_histo[flag];
	}
	return sum / NM;
}


/*-------------------------------------------------------------------------------------------------------------------
Function: var_auto
 Description:get gray's var
 ---------------------------------------------------------------------------------------------------------------------
 Calls: mean_auto
 Called By:windows_otsu_segmentation_auto
 Table Accessed: NONE
 Table Updated:  NONE
 --------------------------------------------------------------------------------------------------------------------
  Input:
@Mat imput_image:mat image you want to get var

  Output：NONE
  Return:mean gray
  Others:NONE
--------------------------------------------------------------------------------------------------------------------*/
float var_auto(Mat input_image)
{
	float var = 0.;
	float mean = mean_auto(input_image);
	float absolute_histo[max_gray_level + 1] = { 0. };
	float NM = input_image.cols*input_image.rows;
	for (int flag_rows = 0; flag_rows < input_image.rows; flag_rows++)
	{
		for (int flag_cols = 0; flag_cols < input_image.cols; flag_cols++)
		{
			int like_hood = (int)input_image.at<uchar>(flag_rows, flag_cols);
			absolute_histo[like_hood]++;
		}
	}
	for (int flag = 0; flag <= max_gray_level; flag++)
	{
		var += ((float)absolute_histo[flag])*((float)flag - mean)*((float)flag - mean);
	}
	return var/(NM-1);
}


/*-------------------------------------------------------------------------------------------------------------------
Function:windows_otsu_segmentation_auto
 Description:threshold segmentation with an windows otsu method segmentation autoly
 ---------------------------------------------------------------------------------------------------------------------
 Calls: globel_otsu_segmentation_auto var_auto
 Called By:main_segmentation.cpp
 Table Accessed: NONE
 Table Updated:  NONE
 --------------------------------------------------------------------------------------------------------------------
  Input:
@Mat imput_image:mat image you want to segmentate
@Mat output_image:output Mat image
@int filter_half_wide:window wide
  Output：output Mat image had been segmentated
  Return:NONE
  Others:NONE
--------------------------------------------------------------------------------------------------------------------*/
void windows_otsu_segmentation_auto(Mat input_image, Mat &output_image,int filter_half_wide)
{
	output_image.create(input_image.rows, output_image.cols, input_image.type());
	Mat filter_window;
	Mat tp_filter_window;
	float threshold;
	float var;

	for (int count_cols = 0; count_cols <= input_image.cols-1; count_cols++)
	{
		//开两个滑动窗口
		filter_window = input_image(
			Range(
				1, input_image.rows
					  ),
			Range(
			(int)max_2_num(1.0, (float)(count_cols - filter_half_wide)),
			(int)min_2_num(input_image.cols, (float)(count_cols + filter_half_wide))
					  )
													  );
		tp_filter_window = input_image(
			Range(
				1, input_image.rows
					),
			Range(
			(int)max_2_num(1.0, (float)(count_cols - filter_half_wide)),
			(int)min_2_num(input_image.cols, (float)(count_cols + filter_half_wide))
			)
												     );
		//求阈值和方差
		threshold=globel_otsu_segmentation_auto(filter_window, tp_filter_window);
		var = var_auto(filter_window);
		cout << var<< endl;
		cout << threshold << endl;
		//二值化
		if (var > threshold)
		{
			for (int flag = 0; flag < input_image.rows;flag++)
			{
				if (input_image.at<uchar>(flag, count_cols) > threshold)
				{
					output_image.at<uchar>(flag, count_cols) = max_gray_level;
				}
				else
				{
					output_image.at<uchar>(flag, count_cols) = 0;
				}
			}
		}
		else
		{
			for (int flag = 0; flag < input_image.rows; flag++)
			{
				output_image.at<uchar>(flag, count_cols) =max_gray_level;
			}
		}
	}
}