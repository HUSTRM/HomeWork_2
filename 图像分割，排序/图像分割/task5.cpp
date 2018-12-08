#include<opencv2/opencv.hpp>
using namespace cv;
void segment2()
{
	int n = 20;
	Mat segment = imread("task52.jpg", 0);
	float m_pre = segment.at<uchar>(0, 0) / n, m_now;
	for (int i = 0; i < segment.rows; i++)
	{
		for (int j = 0; j < segment.cols; j++)
		{
			float diff = 0;
			int index = i * segment.rows + j;
			if (index <= n)
				diff = segment.at<uchar>(i, j);
			else
			{
				if(j>=n+1)
					diff = segment.at<uchar>(i, j)- segment.at<uchar>(i, j-n-1);
				else
					diff = segment.at<uchar>(i, j) - segment.at<uchar>(i-1, segment.cols-n+j-1);

			}
			diff *= 1 / n;
			m_now = m_pre + diff;
			m_pre = m_now;
			if (segment.at<uchar>(i, j) > cvRound(0.5*m_now))
				segment.at<uchar>(i, j) = 255;
			else
				segment.at<uchar>(i, j) = 0;
		}
	}
	imshow("output", segment);
		imwrite("output52.jpg", segment);
		waitKey();
}