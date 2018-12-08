#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#define WINDOWNAME "阈值处理"
int threshold_value;
Mat src;
static void  on_threshold(int, void*);
int main()
{
	src = imread("1.tif");
	cvtColor(src, src, COLOR_BGR2GRAY);
	namedWindow(WINDOWNAME, 1);
	createTrackbar("阈值", WINDOWNAME, &threshold_value, 255, on_threshold);
	waitKey(0);
	return 0;
}
static void on_threshold(int, void*)
{
	int width = src.cols;
	int height = src.rows;
	Mat dst = Mat::zeros(height, width, CV_8UC1); for (int row = 0; row != dst.rows; row++)
		for (int col = 0; col != dst.cols; col++)
		{
			if (src.at<uchar>(row, col) >= threshold_value)
				dst.at<uchar>(row, col) = 255;
		}
	imshow(WINDOWNAME, dst);
}