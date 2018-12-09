#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;
int Otsu(Mat Image);
int main() {
	Mat src = imread("polymersomes.png");
	Mat gray(src.size(), CV_8UC1);
	for (int i = 0;i < src.rows;i++)
		for (int j = 0;j < src.cols;j++) {
			gray.at<uchar>(i, j) = src.at<Vec3b>(i, j)[0];
		}
	imshow("gray", gray);
	int threshold = Otsu(gray);
	cout << threshold;
	for (int i = 0;i < gray.rows;i++)
		for (int j = 0;j < gray.cols;j++) {
			if (gray.at<uchar>(i, j) < threshold)
				gray.at<uchar>(i, j) = 0;
			else gray.at<uchar>(i, j) = 255;
		}
	namedWindow("task3");
	imshow("task3", gray);
	imwrite("task3.png", gray);
	waitKey(0);
	return 0;

}
int Otsu(Mat image) {
	int threshold = 0;
	int hist[256] = { 0 };
	int r = image.rows;
	int c = image.cols;
	for (int i = 0;i < r;i++)
		for (int j = 0;j < c;j++) {
			int p = image.at<uchar>(i, j);
			hist[p]++;
		}
	//for (int i = 0;i < 256;i++) {
		//cout << hist[i];
		//cout << endl;
	//}
	float size = r * c*1.0;
	//cout << size;
	float pHist[256] = { 0 };
	for (int i = 0;i < 256;i++) {
		pHist[i] =(float)hist[i] * 1.0 / size;

	}
	float maxG = 0.0;
	for (int i = 0;i < 256;i++) {
	float u = 0.0;
	float u0 = 0.0, u1 = 0.0;
	float w0 = 0.0, w1 = 0.0;
	float sum0 = 0.0;
	float sum1 = 0.0;
		for (int j = 0;j <256;j++) {
			if (j <= i) {
				w0 += pHist[j];
				sum0 += j * pHist[j] ;
			}
			else {
				w1 += pHist[j];
				sum1 += j * pHist[j] ;
			}
			}
		u0 = sum0 /w0;
		u1 = sum1 / w1;
		float g =w0 *w1* pow((u0 - u1), 2);
		if (g > maxG) {
			maxG = g;
			threshold = i;

		}
	}
	return threshold;
}