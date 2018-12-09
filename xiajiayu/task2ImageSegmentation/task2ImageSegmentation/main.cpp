#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int ThresHold(Mat image);
int main() {
	Mat src = imread("polymersomes.png");
	Mat gray(src.size(), CV_8UC1);
	for(int i=0;i<src.rows;i++)
		for (int j = 0;j < src.cols;j++)
		{
			gray.at<uchar>(i, j) = src.at<Vec3b>(i, j)[0];
		}	
	imshow("gray", gray);
	int T = ThresHold(gray);
	for(int i=0;i<src.rows;i++)
		for (int j = 0;j < src.cols;j++) {
			if (gray.at<uchar>(i, j) < T) {
				gray.at<uchar>(i, j) = 0;
			}
			else
				gray.at<uchar>(i, j) = 255;
		}
	imshow("task2", gray);
	imwrite("task2.png", gray);
	waitKey(0);
	return 0;

}
int ThresHold(Mat image) {
	int Hist[256] = { 0 };
	float T0 = 0, T1 = 0, T2 = 0,T=0;
	int num1 = 0, num2 = 0;
	float r = image.rows, c = image.cols;
	
	for(int i=0;i<r;i++)
		for (int j = 0;j < c;j++) {
			int p = image.at<uchar>(i, j);
			Hist[p]++;
		}
	for (int i = 0;i < 256;i++) {
		T0 += Hist[i] * i;

	}
	T0 = T0 / (r*c);
	cout << T0;
	while (true) {
		for (int i = 0;i < T0;i++) {
			T1 += Hist[i] * i;
			num1 += Hist[i];
		}
		if (num1 == 0)
			continue;
		for (int j = T0 + 1;j < 256;j++) {
			T2 += Hist[j] * j;
			num2 += Hist[j];
		}
		if (num2 == 0)
			continue;
		T = (T1/num1+ T2/num2) / 2;
		if (T == T0) {
			break;
		}
		else
			T0 =T;

	}
	cout << T0;
	return T0;

}