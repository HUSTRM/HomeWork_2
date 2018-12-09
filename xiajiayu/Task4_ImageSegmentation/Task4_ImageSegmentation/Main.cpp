#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
Mat ThresHold(Mat image,int row,int col);
int Otsu(Mat image);
int main() {
	Mat src = imread("septagon.png");
	imshow("src", src);
	Mat gray(src.size(), CV_8UC1);
	for(int i=0;i<src.rows;i++)
		for (int j = 0;j < src.cols;j++) {
			gray.at<uchar>(i, j) = src.at<Vec3b>(i, j)[0];
		};
	imshow("gray",gray);
	Mat dst = ThresHold(gray,2,2);
	imshow("task4", dst);
	imwrite("task4.png", dst);
	waitKey(0);
	return 0;
}
Mat ThresHold(Mat image,int row,int col) {
	int r = image.rows;
	int c = image.cols;
	int dr = r / row, dc = c / col;
	int deltR = (r % row)/2, deltC = (c % col)/2;

	for (int i = 0;i < row;i++) {
		for (int j = 0;j < col;j++) {
			int x = dc * j + deltC ,y= dr * i + deltR;
			Mat seg=image(Rect(x, y, dc, dr));
			int t =Otsu(seg);
			cout << t<<endl;
			for(int k=0;k<seg.rows;k++)
				for (int l = 0;l < seg.cols;l++) {
					if (seg.at<uchar>(k, l) < t) {
						seg.at<uchar>(k, l) = 0;
					}
					else
						seg.at<uchar>(k, l) = 255;
				}

		}
	}
	return image;
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
	float phist[256] = { 0 };
	for (int i = 0;i < 256;i++) {
		phist[i] = (float)hist[i] * 1.0 / size;

	}
	float maxG = 0.0;
	for (int i = 0;i < 256;i++) {
		float u = 0.0;
		float u0 = 0.0, u1 = 0.0;
		
		float w0 = 0.0, w1 = 0.0;
		float sum0 = 0.0;
		float sum1 = 0.0;
		for (int j = 0;j < 256;j++) {
			if (j <= i) {
				w0 += phist[j];
				sum0 += j * phist[j];
			}
			else {
				w1 +=phist[j];
				sum1 += j * phist[j];
			}
		}
		u0 = sum0 / w0;
		u1 = sum1 / w1;
		//cout << u0;
		//cout << u1;
		//cout << endl;
		float g = w0 * w1* pow((u0 - u1), 2);
		//cout << g;
		if (g > maxG) {
			maxG = g;
			threshold = i;
		}
	}
	return threshold;
}