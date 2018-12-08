#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int a[400];
int index = 0;
float global_gray_variance=0;
float global_gray_average;
int pixelcount[256];
float pixelpro[256];
int sign = 0;
float sigamaBC = 0;
float SF;
Mat image = imread("6.tif", 0);
int pixelsum = image.rows*image.cols;//像素点总个数
void otsu_thresholding(int low_pixel_value,int high_pixel_value);
int main()
{
	otsu_thresholding(0,255);
	Mat result = Mat::zeros(image.rows, image.cols, CV_8UC1);
	int i, j, k, n;
	for (i = 1; i < 400; i++)
	{
		if (a[i] == 0)
		{
			n = i;
			break;
		}
	}
	for (i = 0; i < index; i++)
	{
		cout << a[i] << endl;
	}
	int b[400];
	for (i = 0; i < n; i++)
	{
		b[i] = (i+1) * 255 / n;
	}
	for (i = 0; i < image.rows; i++)
	{
		for (j = 0; j < image.cols; j++)
		{
			for (k = 0; k < n; k++)
			{
				if (int(image.at<uchar>(i, j)) < b[k])
				{
					result.at<uchar>(i, j) = uchar(b[k]);
					break;
				}
			}
		}
	}
	imshow("多阈值处理", result);
	waitKey(0);
	return 0;
}
void otsu_thresholding(int low_pixel_value,int high_pixel_value)
{
	int i, j;
	for (i = low_pixel_value; i <= high_pixel_value; i++)
		pixelcount[i] = 0;
	for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++)
		{
			if (int(image.at<uchar>(i, j)) <= high_pixel_value && int(image.at<uchar>(i, j)) >= low_pixel_value)
				pixelcount[int(image.at<uchar>(i, j))]++;
		}
	for (i = low_pixel_value; i <= high_pixel_value ; i++)
		pixelpro[i] = float(pixelcount[i]) / pixelsum;
	float gray_average1;//小于等于当前像素值的所有像素点的像素值的平均值(以占pixelsum的比例计算)
	float gray_average2;//大于当前像素值的所有像素点的像素值的平均值(以占pixelsum的比例计算)
	float pixelprosum1;//小于当前像素值的所有像素点的比率和
	float pixelprosum2;//大于当前像素值的所有像素点的比率和
	float gray_average1_real;//小于等于当前像素值的所有像素点的像素值的平均值
	float gray_average2_real; //大于当前像素值的所有像素点的像素值的平均值
	float gray_average;//所有像素点的像素值的平均值
	float gray_variancemix;//类间方差
	float gray_variancemax = 0;//最大方差
	int threshold_value = 0;
	for (i = low_pixel_value; i <= high_pixel_value; i++)
	{
		gray_average1 = gray_average2 = gray_average = pixelprosum1 = pixelprosum2 = gray_variancemix = 0;
		for (j = low_pixel_value; j <= high_pixel_value; j++)
		{
			if (j <= i)
			{
				gray_average1 = pixelpro[j] * j;
				pixelprosum1 += pixelpro[j];
			}
			else
			{
				gray_average2 = pixelpro[j] * j;
				pixelprosum2 += pixelpro[j];
			}
		}
		gray_average1_real = gray_average1 / pixelprosum1;
		gray_average2_real = gray_average2 / pixelprosum1;
		gray_average = gray_average1 + gray_average2;
		if (sign == 0)
		{
			for(i=0;i<image.rows;i++)
				for (j = 0; j < image.cols; j++)
				{
					global_gray_variance += 1.0 / pixelsum * pow(int(image.at<uchar>(i, j))- gray_average, 2);
				}
			global_gray_average = gray_average;
			sign=1;
		}
		gray_variancemix = pixelprosum1 * pow(gray_average1 - global_gray_average, 2) + pixelprosum2 * pow(gray_average2 - global_gray_average, 2);
		if (gray_variancemix > gray_variancemax)
		{
			gray_variancemax = gray_variancemix;
			threshold_value = i;
		}
	}
	a[index++] = threshold_value;
	float gray_variance1 = 0;//类内方差1
	float gray_variance2 = 0;//类内方差2
		gray_average1 = gray_average2 = gray_average = pixelprosum1 = pixelprosum2 = gray_variancemix = 0;
		for (j = low_pixel_value; j <= high_pixel_value; j++)
		{
			if (j <= threshold_value)
			{
				gray_average1 = pixelpro[j] * j;
				pixelprosum1 += pixelpro[j];
			}
			else
			{
				gray_average2 = pixelpro[j] * j;
				pixelprosum2 += pixelpro[j];
			}
		}
		gray_average1_real = gray_average1 / pixelprosum1;
		gray_average2_real = gray_average2 / pixelprosum1;
		gray_average = gray_average1 + gray_average2;
		for (i = low_pixel_value; i <= high_pixel_value; i++)
		{
			if (i < threshold_value)
			{
				gray_variance1 += pixelcount[i] / (pixelprosum1*pixelsum)*pow(i - gray_average1_real, 2);
			}
			else
			{
				gray_variance2 += pixelcount[i] / (pixelprosum2*pixelsum)*pow(i - gray_average2_real, 2);
			}
		}
		if (gray_variance1 < gray_variance2)
		{
			sigamaBC += pixelprosum1 * pow(gray_average1 - global_gray_average, 2) + pixelprosum2 * pow(gray_average2 - global_gray_average,2);
			SF = sigamaBC / global_gray_variance;
			if (SF > 0.9)
			{
				return;
			}
			else
			{
				sigamaBC -= pixelprosum2 * pow(gray_average2 - global_gray_average, 2);
				otsu_thresholding(threshold_value, high_pixel_value);
			}
		}
		else
		{
			sigamaBC += pixelprosum2 * pow(gray_average2 - gray_average, 2)+ pixelprosum1 * pow(gray_average1 - global_gray_average, 2);
			SF = sigamaBC / global_gray_variance;
			if (SF > 0.9)
			{
				return;
			}
			else
			{
				sigamaBC -= pixelprosum2 * pow(gray_average2 - global_gray_average, 2);
				otsu_thresholding(low_pixel_value, threshold_value); 
			}
		}
}