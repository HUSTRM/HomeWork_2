#include<opencv2\opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
int main()
{
	Mat src = imread("1.tif");
	cvtColor(src, src, COLOR_BGR2GRAY);
	int width = src.cols;
	int height = src.rows;
	Mat dst = Mat::zeros(height, width, CV_8UC1);
	cout << "请输入规定的阈值(0-255):" << endl;
	int threshold_value;
	cin >> threshold_value;
	for (int row = 0; row != dst.rows; row++)
		for (int col = 0; col != dst.cols; col++)
		{
			if(src.at<uchar>(row,col)>=threshold_value)
			    dst.at<uchar>(row, col) = 255;
		}
	imshow("阈值处理", dst);
	waitKey(0);
	return 0;
}
