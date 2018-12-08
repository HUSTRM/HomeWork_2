#include<opencv2/opencv.hpp>
using namespace cv;
void manualThreshhold()
{
	Mat manual = imread("task1.jpg",0);
	int threshhold = 144;
	for (int i = 0; i < manual.rows; i++)
	{
		for (int j = 0; j < manual.cols; j++)
		{
			if (manual.at<uchar>(i, j) < threshhold)
				manual.at<uchar>(i, j) = 255;
			else
			{
				manual.at<uchar>(i, j) = 0;
			}
		}
	}
	imshow("output",manual);
	imwrite("output1.jpg", manual);
	waitKey();
}