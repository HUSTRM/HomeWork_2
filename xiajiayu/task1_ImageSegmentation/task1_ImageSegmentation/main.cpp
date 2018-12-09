#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

int main() {
	Mat src = imread("polymersomes.png");
	int threshold = 140;
	Mat dst(src.size(),CV_8UC3);
	/*for(int i=0;i<src.rows;i++)
		for (int j = 0;j < src.cols;j++) {
			cout << int(src.at<Vec3b>(i, j)[0]);
			cout << int(src.at<Vec3b>(i, j)[1]);
			cout << int(src.at<Vec3b>(i, j)[2]);
			cout << endl;
		}*/
	for (int i = 0;i < src.rows;i++) {
		for (int j = 0;j < src.cols*3;j++) {
			if (src.at<uchar>(i, j) < threshold) {
				dst.at<uchar>(i, j) = 0;
			}
			else {
				dst.at<uchar>(i, j) = 255;
			}

		}
	}
	namedWindow("srcImage");
	namedWindow("task1");
	imshow("srcImage", src);
	imshow("task1", dst);
	imwrite("task1.png", dst);

	waitKey(0);
	return 0;
}