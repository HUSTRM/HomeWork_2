#pragma once
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define STR_WINNAME "Image Segmentation"
#define DBL_BASICGB_ACCU 0.001
#define INT_BASICGB_MINPLUS 200
#define GOTO system("pause > nul");break

struct sbgRet {
	Mat out;
	int threshold;
};
struct soRet {
	Mat out;
	int threshold;
	double separability;
};
struct ssRet {
	Mat out;
	vector<int> thres;
	vector<double> sep;
};

double getWeightedGS(vector<int> hist, int from, int to);
vector<int> getHist(Mat in);
vector<double> getHistNorm(vector<int> hist);
Mat segMan(Mat in, int thres);
sbgRet segBasicG(Mat in, double accu);
soRet segOtsu(Mat in);
ssRet segPart(Mat in);