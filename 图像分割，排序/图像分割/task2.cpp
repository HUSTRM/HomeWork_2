#include<opencv2/opencv.hpp>
using namespace cv;
void autoThreshhold()
{
	Mat autoT = imread("task1.jpg", 0);
	double threshhold = 144;
	double dealta = 0.1;
	int sum1 = 0, sum2 = 0;
	int count1 = 0, count2 = 0;
	double mean1 = 0, mean2 = 0;
	while (true)
	{
		for (int i = 0; i < autoT.rows; i++)
		{
			for (int j = 0; j < autoT.cols; j++)
			{
				if (autoT.at<uchar>(i, j) < threshhold)
				{
					sum1 += autoT.at<uchar>(i, j);
					count1++;
					mean1 = (double)sum1 / count1;
				}
				else
				{
					sum2 += autoT.at<uchar>(i, j);
					count2++;
					mean2 = (double)sum2 / count2;
				}
			}
		}
		double thresh = 0.5*(mean1 + mean2);
		if (abs(thresh - threshhold) <= dealta)
		{
			threshhold = thresh;
			break;
		}
		else
		{
			threshhold = thresh;
		}
	}
	for (int i = 0; i < autoT.rows; i++)
	{
		for (int j = 0; j < autoT.cols; j++)
		{
			if (autoT.at<uchar>(i, j) < threshhold)
				autoT.at<uchar>(i, j) = 255;
			else
			{
				autoT.at<uchar>(i, j) = 0;
			}
		}
	}
	imshow("output", autoT);
	imwrite("output2.jpg", autoT);
	waitKey();
}