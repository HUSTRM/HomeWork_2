// img_seg.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include <string.h>
#include <iostream>
#define HUMAN "手动确定阈值"
#define ROUTE "E:\\opencv\\img_seg\\image_seg\\image_seg\\task.jpg"

using namespace cv;
using namespace std;

Mat src;
int thresh;
void thre_track(int, void *);
void ithreshold(Mat &isrc, Mat &idst, int ithresh, int max_val);
int human_seg();
/*---------------------------------------图像分割部分-------------------------------------*/
void ithreshold(Mat &isrc, Mat &idst, int ithresh, int max_val)
{
	//idst可能需要初始化
	idst = isrc.clone();
	uchar *p, *q;
	for (int i = 0; i < isrc.rows; i++) {
		p = isrc.ptr<uchar>(i);
		q = idst.ptr<uchar>(i);
		for (int j = 0; j < isrc.cols; j++) {
			if (p[j] > ithresh)
				q[j] = max_val;
			else
				q[j] = 0;
		}
	}
}

void thre_track(int, void *)
{
	Mat dst;
	ithreshold(src, dst, thresh, 255);
	imshow(HUMAN, dst);
}

int human_seg()//手动选取阈值 task1
{
	src = imread(ROUTE);
	cvtColor(src, src, COLOR_BGR2GRAY);
	if (!src.data)
		return 0;
	namedWindow(HUMAN, WINDOW_AUTOSIZE);
	createTrackbar("阈值：", HUMAN, &thresh, 255, thre_track);
	thre_track(thresh, 0);
	waitKey(0);
	return 0;
}

int iteration()		//迭代法获得阈值 task2
{
	int histogram[256] = { 0 };
	int gray_min, gray_max, new_thresh, gray_sum, point_sum;
	gray_min = 255;
	gray_max = 0;
	gray_sum = 0;
	point_sum = 0;
	src = imread(ROUTE);
	cvtColor(src, src, COLOR_BGR2GRAY);
	Mat img = src.clone();
	for (int i = 0; i<img.rows; i++)
		for (int j = 0; j < img.cols; j++)
		{
			int x = img.at<uchar>(i, j);
			if (x > gray_max)
				gray_max = x;
			if (x < gray_min)
				gray_min = x;
			histogram[x]++;		//统计灰度图直方图
		}
	thresh = 0;
	new_thresh = (gray_max + gray_min) / 2;
	while (thresh != new_thresh)
	{
		thresh = new_thresh;
		int min_mean, max_mean;
		for (int i = gray_min; i < thresh; i++) {
			gray_sum += (histogram[i] * i);
			point_sum += histogram[i];
		}
		min_mean = gray_sum / point_sum;
		gray_sum = 0;
		point_sum = 0;
		for (int j = thresh + 1; j <= gray_max; j++) {
			gray_sum += (histogram[j] * j);
			point_sum += histogram[j];
		}
		max_mean = gray_sum / point_sum;
		gray_sum = 0;
		point_sum = 0;
		new_thresh = (min_mean + max_mean) / 2;
	}
	cout << "The thresh I find is :" << thresh << endl;
	Mat dst;
	ithreshold(src, dst, thresh, 255);
	imshow("迭代", dst);
	return 1;
}

int otsu()		//大津法Otsu
{
	int histogram[256] = { 0 };
	int gray_max, gray_min, i, j, size;
	int	g_sum_min, g_sum_max, p_sum_min, p_sum_max;//存储前景、背景的灰度和与个数和
	double w0 = 0, w1 = 0;		 //前景和背景所占整幅图像的比例  
	double u0 = 0, u1 = 0;		 //前景和背景的平均灰度
	double u = 0;					//图像总平均灰度
	double variance = 0;
	double max_variance = 0;//最大类间方差
	gray_max = 0;
	gray_min = 255;
	src = imread(ROUTE);
	cvtColor(src, src, COLOR_BGR2GRAY);
	Mat img = src.clone();
	size = (img.cols)*(img.rows);		//获得图像大小
	for (i = 0; i<img.rows; i++)
		for (int j = 0; j < img.cols; j++)
		{
			int x = img.at<uchar>(i, j);
			if (x > gray_max)
				gray_max = x;
			if (x < gray_min)
				gray_min = x;
			histogram[x]++;		//统计灰度图直方图
		}
	for (i = gray_min; i <= gray_max; i++)
	{
		g_sum_max = 0;
		g_sum_min = 0;
		p_sum_max = 0;
		p_sum_min = 0;
		w0 = 0;
		w1 = 0;
		for (j = gray_min; j < i; j++) {
			g_sum_min += j * histogram[j];
			p_sum_min += histogram[j];
		}
		u0 = (double)g_sum_min / p_sum_min;
		w0 = (double)p_sum_min / size;
		for (j = i; j <= gray_max; j++) {
			g_sum_max += histogram[j] * j;
			p_sum_max += histogram[j];
		}
		u1 = (double)g_sum_max / p_sum_max;
		w1 = 1 - w0;
		u = u0 * w0 + u1 * w1;
		variance = w0 * w1*(u0 - u1)*(u0 - u1);
		if (variance > max_variance) {
			max_variance = variance;
			thresh = i;
		}
	}
	cout << "According to Otsu,the thresh is:" << thresh << endl;
	Mat dst;
	ithreshold(img, dst, thresh, 255);
	imshow("Otsu", dst);
	return 0;
}


int local_Otsu(int max_val, int parts)//遍历图片像素，设置条件当像素不为0时记下此点得像素值，累计像素值除以累计个数就是均值，再来一次遍历  同样得条件 算二阶中心矩累计和 最后除以累计像素个数
{									//parts表示所分区块数目 max_val表示最大灰度值
	if (parts < 2)
		return -1;
	src = imread(ROUTE);
	cvtColor(src, src, COLOR_BGR2GRAY);
	Mat dst;
	dst = src.clone();
	int i, j, k, g_sum_max, g_sum_min, p_sum_max, p_sum_min, size;
	int gray_min = 255, gray_max = 0;
	int cols = src.cols;
	int rows = src.rows;
	int histogram[256] = { 0 };
	int delta = cols / parts;
	size = delta * rows;
	double w0 = 0, w1 = 0;		 //前景和背景所占整幅图像的比例  
	double u0 = 0, u1 = 0;		 //前景和背景的平均灰度
	double u = 0;					//图像总平均灰度
	double variance = 0;
	double max_variance = 0;//最大类间方差
	for (i = 0; i < parts - 1; i++) {
		for (j = delta * i; j<delta*(i + 1); j++)
			for (k = 0; k < rows; k++) {
				int x = src.at<uchar>(k, j);
				if (x > gray_max)
					gray_max = x;
				if (x < gray_min)
					gray_min = x;
				histogram[x]++;		//统计灰度图直方图
			}
		for (j = gray_min; j <= gray_max; j++) {
			g_sum_max = 0;
			g_sum_min = 0;
			p_sum_max = 0;
			p_sum_min = 0;
			w0 = 0;
			w1 = 0;
			for (k = gray_min; k < j; k++) {
				g_sum_min += k * histogram[k];
				p_sum_min += histogram[k];
			}
			u0 = (double)g_sum_min / p_sum_min;
			w0 = (double)p_sum_min / size;
			for (k = j; k <= gray_max; k++) {
				g_sum_max += histogram[k] * k;
				p_sum_max += histogram[k];
			}
			u1 = (double)g_sum_max / p_sum_max;
			w1 = 1 - w0;
			u = u0 * w0 + u1 * w1;
			variance = w0 * w1*(u0 - u1)*(u0 - u1);
			if (variance > max_variance) {
				max_variance = variance;
				thresh = j;
			}
		}
		for (j = delta * i; j<delta*(i + 1); j++)
			for (k = 0; k < rows; k++) {		//二值化
				if (src.at<uchar>(k, j) > thresh)
					dst.at<uchar>(k, j) = max_val;
				else
					dst.at<uchar>(k, j) = 0;
			}
		memset(histogram, 0, 256);
		max_variance = variance = 0;
		gray_min = 255;
		gray_max = 0;
	}
	size = cols - delta * (parts - 1);
	for (j = delta * (parts - 1); j<cols; j++)//对最后一区块进行操作
		for (k = 0; k < rows; k++) {
			int x = src.at<uchar>(k, j);
			if (x > gray_max)
				gray_max = x;
			if (x < gray_min)
				gray_min = x;
			histogram[x]++;		//统计灰度图直方图
		}
	for (j = gray_min; j <= gray_max; j++) {
		g_sum_max = 0;
		g_sum_min = 0;
		p_sum_max = 0;
		p_sum_min = 0;
		w0 = 0;
		w1 = 0;
		for (k = gray_min; k < j; k++) {
			g_sum_min += k * histogram[k];
			p_sum_min += histogram[k];
		}
		u0 = (double)g_sum_min / p_sum_min;
		w0 = (double)p_sum_min / size;
		for (k = j; k <= gray_max; k++) {
			g_sum_max += histogram[k] * k;
			p_sum_max += histogram[k];
		}
		u1 = (double)g_sum_max / p_sum_max;
		w1 = 1 - w0;
		u = u0 * w0 + u1 * w1;
		variance = w0 * w1*(u0 - u1)*(u0 - u1);
		if (variance > max_variance) {
			max_variance = variance;
			thresh = j;
		}
	}
	for (j = delta * (parts - 1); j<cols; j++)
		for (k = 0; k < rows; k++) {		//二值化
			if (src.at<uchar>(k, j) > thresh)
				dst.at<uchar>(k, j) = max_val;
			else
				dst.at<uchar>(k, j) = 0;
		}
	imshow("locally_Otsu", dst);
}


void move_seg()//移动平均法
{
	src = imread(ROUTE);
	cvtColor(src, src, COLOR_BGR2GRAY);
	uchar *p, *q;
	Mat dst = src.clone();
	float n, m_pre, m_now, temp;
	int num, ithresh, sum = -1;
	cout << src.rows << endl;
	cout << "Input the num" << endl;
	cin >> num;
	n = (float)num;
	m_pre = src.at<uchar>(0, 0) / n;
	for (int i = 0; i < src.rows; i++) {
		p = src.ptr<uchar>(i);
		q = dst.ptr<uchar>(i);
		for (int j = 0; j < src.cols; j++) {
			float dif;
			sum++;
			if (sum < num + 1)
				dif = src.at<uchar>(i, j);//p[j];
			else {
				if (j < num + 1)
					dif = src.at<uchar>(i, j) - src.at<uchar>(i - 1, j - num - 1 + src.cols);//p[j] - p[j - num - 1];
				else
					dif = src.at<uchar>(i, j) - src.at<uchar>(i, j - num - 1);
			}
			dif = dif / n;
			m_now = m_pre + dif;
			m_pre = m_now;//????
			temp = m_now * 0.75;//参数加权
			ithresh = (int)temp;
			temp -= ithresh;
			if (temp > 0.5)
				ithresh++;
			if (p[j] > ithresh)
				q[j] = 255;
			else
				q[j] = 0;
		}
		cout << ithresh << endl;
	}
	imshow("source", src);
	imshow("move_seg", dst);
	waitKey(0);
}

void multi_seg()//先利用Otsu算法将图片分为两类，取类内方差最大的继续使用Otsu算法分开，直到类间方差与图像总方差比值满足一定条件
{
	src = imread(ROUTE);
	int histogram[256] = { 0 };
	int threshs[256] = { 0 };
	int q, gray_max, gray_min, i, j, size;
	double gray_ave, vt;
	int g_sum_min, g_sum_max, p_sum_min, p_sum_max;
	double var_max = 0;//每一类的类内方差及最大方差
	double var_all;//类间方差
	double w0 = 0, w1 = 0; //前景和背景所占整幅图像的比例  
	double u0 = 0, u1 = 0; //前景和背景的平均灰度
	double u = 0;
	double variance = 0;
	double max_variance = 0;//最大类间方差
	double SF;
	gray_ave = 0;//图像的平均灰度值
	gray_max = 0;
	gray_min = 255;
	vt = 0;//vt为图像的总方差
	SF = 0;//SF为结束条件,SF=omeg/vt
	q = 1;
	cvtColor(src, src, COLOR_BGR2GRAY);
	Mat img = src.clone();
	size = (img.cols)*(img.rows);
	for (i = 0; i < img.rows; i++) //获得灰度直方图
		for (j = 0; j < img.cols; j++) {
			int x = img.at<uchar>(i, j);
			if (x > gray_max)
				gray_max = x;
			if (x < gray_min)
				gray_min = x;
			histogram[x]++;
		}
	threshs[0] = gray_min;
	threshs[1] = gray_max;
	for (i = gray_min; i <= gray_max; i++)
		gray_ave += (histogram[i] * i);
	gray_ave /= size;
	for (i = gray_min; i <= gray_max; i++)
		vt = vt + histogram[i] * (i - gray_ave)*(i - gray_ave);
	vt /= size;
	while (SF<0.95)//SF>0.95认为分割较为完全
	{
		SF = 0;
		max_variance = 0;
		int isize = 0;
		for (i = gray_min; i <= gray_max; i++)
			isize += histogram[i];
		for (i = gray_min; i <= gray_max; i++)
		{
			g_sum_max = 0;
			g_sum_min = 0;
			p_sum_max = 0;
			p_sum_min = 0;
			w0 = 0;
			w1 = 0;
			for (j = gray_min; j < i; j++) {
				g_sum_min += j * histogram[j];
				p_sum_min += histogram[j];
			}
			u0 = (double)g_sum_min / p_sum_min;
			w0 = (double)p_sum_min / isize;
			for (j = i; j <= gray_max; j++) {
				g_sum_max += histogram[j] * j;
				p_sum_max += histogram[j];
			}
			u1 = (double)g_sum_max / p_sum_max;
			w1 = 1 - w0;
			u = u0 * w0 + u1 * w1;
			variance = w0 * w1*(u0 - u1)*(u0 - u1);
			if (variance > max_variance) {
				max_variance = variance;
				thresh = i;
			}
		}
		int x = 0;
		while (thresh >threshs[x] && x <= q)//直接插入排序
			x++;
		for (int y = q; y >= x; y--)
			threshs[y + 1] = threshs[y];
		threshs[x] = thresh;
		q++;
		var_max = 0;
		double omeg = 0;
		for (x = 0; x < q; x++) {//类内方差统计
			int y;
			int N = 0;
			double ave = 0;
			double var_local = 0;
			for (y = threshs[x]; y <= threshs[x + 1]; y++) {
				N += histogram[y];
				ave += y * histogram[y];
			}
			ave /= N;
			omeg += N * (ave - gray_ave)*(ave - gray_ave);
			for (y = threshs[x]; y <= threshs[x + 1]; y++) {
				var_local += (y - ave)*(y - ave)*histogram[y];

			}
			var_local /= N;
			if (var_local > var_max) {
				var_max = var_local;
				gray_min = threshs[x];
				gray_max = threshs[x + 1];
			}
		}
		omeg /= size;
		SF = omeg / vt;
	}
	//对图像进行分阈值分割
	uchar *a, *b;
	for (i = 0; i < src.rows; i++) {
		a = src.ptr<uchar>(i);
		b = img.ptr<uchar>(i);
		for (j = 0; j < src.cols; j++)
		{
			int s = 0;
			while (a[j] > threshs[s] && s < q)
				s++;
			b[j] = threshs[s + 1];
		}
	}
	imshow("multi_threshold", img);
	imwrite("task6.jpg", img);
}

/*----------------------------------------排序算法部分------------------------------------------*/
void quickSort(int *s, int l, int r)
{
	if (l< r)
	{
		int i = l, j = r, x = s[l];
		while (i < j)
		{
			while (i < j && s[j] >= x) // 从右向左找第一个小于x的数  
				j--;
			if (i < j)
				s[i++] = s[j];
			while (i < j && s[i]< x) // 从左向右找第一个大于等于x的数  
				i++;
			if (i < j)
				s[j--] = s[i];
		}
		s[i] = x;
		quickSort(s, l, i - 1); // 递归调用  
		quickSort(s, i + 1, r);
	}
}

void merge(int *data, int start, int end, int *result)
{
	int left_length = (end - start + 1) / 2 + 1;//左部分区间的数据元素的个数
	int left_index = start;
	int right_index = start + left_length;
	int result_index = start;
	while (left_index < start + left_length && right_index < end + 1)
	{
		//对分别已经排好序的左区间和右区间进行合并
		if (data[left_index] <= data[right_index])
			result[result_index++] = data[left_index++];
		else
			result[result_index++] = data[right_index++];
	}
	while (left_index < start + left_length)
		result[result_index++] = data[left_index++];
	while (right_index < end + 1)
		result[result_index++] = data[right_index++];
}

void merge_sort(int *data, int start, int end, int *result)//归并排序
{
	if (end-start==1)//如果区间中只有两个元素，则对这两个元素进行排序
	{
		if (data[start] > data[end])
		{
			int temp = data[start];
			data[start] = data[end];
			data[end] = temp;
		}
		return;
	}
	else if (end==start)//如果只有一个元素，则不用排序
		return;
	else
	{
		//继续划分子区间，分别对左右子区间进行排序
		merge_sort(data, start, (end - start + 1) / 2 + start, result);
		merge_sort(data, (end - start + 1) / 2 + start + 1, end, result);
		//开始归并已经排好序的start到end之间的数据
		merge(data, start, end, result);
		//把排序后的区间数据复制到原始数据中去
		for (int i = start; i <= end; ++i)
			data[i] = result[i];
	}
}

/*---------------------------------------代码调试部分--------------------------------*/
int main()
{
	int i=0;
	while (i != -1)
	{
		cin >> i;
		switch (i)
		{
		case 1:
			human_seg();//用户选择 task1
			break;
		case 2:
			iteration();//迭代法由程序选择 task2
			break;
		case 3:
			otsu();//Otsu算法
			iteration();//迭代法由程序选择
			break;
		case 4:
			local_Otsu(255, 10);//分块局部适应
			break;
		case 5:
			move_seg();//移动平均法
			break;
		case 6:
			multi_seg();//多阈值最佳分割
			break;
		default:
			break;
		}
	}
	return 0;
}