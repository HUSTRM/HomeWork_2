//
// Created by top on 18-11-26.
//

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int findMax(vector<int> vec) {
    int max = -1;
    for (auto v : vec) {
        if (max < v) max = v;
    }
    return max;
}

int getPosition(vector<int> vec, int num) {
    auto distance = find(vec.begin(), vec.end(), num);
    return distance - vec.begin();
}


void threshold_myself(int threshold){
    Mat src = imread("/home/top/project/ImageSegmentation/imageSegmentation/Picture1.png", IMREAD_GRAYSCALE);
    Mat dst = src;
    for (int i = 0; i < src.rows; ++i) {
          for (int j = 0; j < src.cols; ++j) {
              int value = src.at<uchar>(i, j);
              if (value >= threshold) dst.at<uchar>(i, j) = 255;
              else dst.at<uchar>(i, j) = 0;
           }
    }
    imshow("destiny", dst);
    std::cout << "Done!" << std::endl;
    waitKey(0);
}

void auto_set_threshold() {
    Mat src = imread("/home/top/project/ImageSegmentation/imageSegmentation/Picture1.png", IMREAD_GRAYSCALE);
    Mat dst = src;
    int level = 40;
    int counter[level] = {0};
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            int value = src.at<uchar>(i, j);
            for (int l = 0; l < level; l++) {
                if (value < (l + 1) * 255 / level && value > l * 255 / level) counter[l]++;
                else;
            }
        }
    }
    vector<int> vec;
    for (int ii = 0; ii < level; ii++) {
        cout << counter[ii] << endl;
        vec.push_back(counter[ii]);
    }
    int maxnumber1 = findMax(vec);
    int position1 = getPosition(vec, maxnumber1);
    cout << "Max1:"<<maxnumber1 <<",position1:"<<position1<<endl;
    vector<int>::iterator it;
    for(it=vec.begin(); it!=vec.end();){
        if (*it == maxnumber1) vec.at(position1) = 0;
        ++it;
    }
    int maxnumber2 = findMax(vec);
    int position2 = getPosition(vec, maxnumber2);
    cout << "Max2:"<<maxnumber2 <<",position2:"<<position2<<endl;
    int threshold = (position1+position2)*0.5*255/level;
    cout<<"threshold:"<<threshold<<endl;
    threshold_myself(threshold);
}

int otsuMethod(Mat src){
    const int GrayScale = 256;
//    Mat src = imread("/home/top/project/ImageSegmentation/imageSegmentation/Picture1.png", IMREAD_GRAYSCALE);
    int width = src.rows;
    int height = src.cols;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height, threshold = 0;
    uchar* data = src.ptr<uchar>(0);  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++) //初始化计数数组和比例数组
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
        }
    }

    //计算每个像素在整幅图像中的比例
    float maxPro = 0.0;
    int kk = 0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        if (pixelPro[i] > maxPro)
        {
            maxPro = pixelPro[i];
            kk = i;
        }
    }

    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    /**
     * w0:属于前景的像素点数占整幅图像的比例
     * w1:属于背景的像素点数占整幅图像的比例
     * u0:前景平均灰度,Σ(当前灰度*此灰度出现的概率)/前景像素点数比例
     * u1:背景平均灰度,Σ(当前灰度*此灰度出现的概率)/背景像素点数比例
     * u:图像的总平均灰度
     * deltaTmp:遍历阈值时求得的间类方差
     * deltaMax:目前所得的最大间类方差,随着遍历可能会变
     */
    for (i = 0; i < GrayScale; i++)     // i作为阈值
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++) //遍历每个灰度值，把他们分成前景和背景
        {
            if (j <= i)   //背景部分
            {
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j]; //
            }
            else   //前景部分
            {
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }
        u0 = u0tmp / w0;
        u1 = u1tmp / w1;
        u = u0tmp + u1tmp;
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = i;
        }
    }
    cout<<"OTSU threshold:"<<threshold<<endl;
    return threshold;
}

void my_localAdaptiveThresholding() {
    Mat src = imread("/home/top/project/ImageSegmentation/imageSegmentation/Picture1.png", IMREAD_GRAYSCALE);
    int width = src.rows;
    int height = src.cols;
    int local_row = 10;
    int local_col = 10;
    vector<vector<uchar *>> data;
    for (int ii = 0; ii < local_row; ii++) {
        vector<uchar *> data_one_rows;
        for (int jj = 0; jj < local_col; jj++) {
            data_one_rows.push_back(src.ptr(ii * width / local_row, jj * height / local_col));
            uchar pixel = src.at<uchar>(ii, jj * height / local_col);
            cout << "pixel data:" << pixel << endl;
        }
        data.push_back(data_one_rows);
    }
}

void src_adaptiveThreshold()
{

//    Mat src = _src.getMat();
    Mat src = imread("/home/top/project/ImageSegmentation/"
                     "imageSegmentation/Picture4.png", IMREAD_GRAYSCALE);
    Size size = src.size();
    blur(src,src,Size(5,5));
    Mat dst = Mat(size, src.type());

    double maxValue = 255;
    if( maxValue < 0 )
    {
        dst = Scalar(0);
        return;
    }

    Mat mean;

    if( src.data != dst.data )
        mean = dst;

    int blockSize = 71;
    double delta = 4;

//    boxFilter( src, mean, src.type(), Size(blockSize, blockSize),
//               Point(-1,-1), true, BORDER_REPLICATE|BORDER_ISOLATED );

    Mat srcfloat,meanfloat;
    src.convertTo(srcfloat,CV_32F);
    meanfloat=srcfloat;
    GaussianBlur(srcfloat, meanfloat, Size(blockSize, blockSize), 0, 0, BORDER_REPLICATE|BORDER_ISOLATED);
    meanfloat.convertTo(mean, src.type());


    int type = CV_THRESH_BINARY;
    int i, j;
    uchar imaxval = saturate_cast<uchar>(maxValue);
    int idelta = type == THRESH_BINARY ? cvCeil(delta) : cvFloor(delta);
    uchar tab[768];

    if( type == CV_THRESH_BINARY )
        for( i = 0; i < 768; i++ )
            tab[i] = (uchar)(i - 255 > -idelta ? imaxval : 0);
//    else( type == CV_THRESH_BINARY_INV )
//        for( i = 0; i < 768; i++ )
//            tab[i] = (uchar)(i - 255 <= -idelta ? imaxval : 0);

    if( src.isContinuous() && mean.isContinuous() && dst.isContinuous() )
    {
        size.width *= size.height;
        size.height = 1;
    }

    for( i = 0; i < size.height; i++ )
    {
        const uchar* sdata = src.ptr(i);
        const uchar* mdata = mean.ptr(i);
        uchar* ddata = dst.ptr(i);

        for( j = 0; j < size.width; j++ )
            ddata[j] = tab[sdata[j] - mdata[j] + 255];
    }
    imshow("src", src);
    imshow("dst", dst);
    waitKey(0);
}

void blockedThresholdSegmentation(){
    Mat src = imread("/home/top/project/ImageSegmentation/"
                     "imageSegmentation/Picture4.png", IMREAD_GRAYSCALE);
    Size size = src.size();
    int width = src.cols;
    int height = src.rows;
    Mat dst = Mat(size, src.type());
    imshow("dst", dst);
    waitKey(0);
    Mat roi_image;
    int rows = 10, cols = 10;
    Point topleft, bottomright;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            topleft.x = cvRound(j*width/(float)cols);
            topleft.y = cvRound(i*height/(float)rows);
            bottomright.x = cvRound((j+1)*width/(float)cols);
            bottomright.y = cvRound((i+1)*height/(float)rows);
            int w = bottomright.x-topleft.x;
            int h = bottomright.y-topleft.y;
            Rect rect(topleft.x,topleft.y,w,h);
            roi_image = src(rect);
            Mat roi_image_bi(roi_image.size(), roi_image.type());

            int threshold = otsuMethod(roi_image);

            for (int i = 0; i < roi_image.rows; ++i) {
                for (int j = 0; j < roi_image.cols; ++j) {
                    int value = roi_image.at<uchar>(i, j);
                    if (value >= threshold) roi_image_bi.at<uchar>(i, j) = 255;
                    else roi_image_bi.at<uchar>(i, j) = 0;
                }
            }

            roi_image_bi.copyTo(dst(rect));
            imshow("dst", dst);
            waitKey(0);
        }
    }
    imshow("dst", dst);
    waitKey(0);

}

void averMovingSegmentation(){
    Mat src = imread("/home/top/project/ImageSegmentation/"
                     "imageSegmentation/Picture5.png", IMREAD_GRAYSCALE);
    Mat dst(src.size(), src.type());
    uchar* dstdata = dst.ptr<uchar>(0);
    uchar* data = src.ptr<uchar>(0);
    int w = src.cols, h = src.rows;
    int step = src.step;
    int num = 20;
    float n = (float)num;
    float m_pre = data[0]/n, m_now;
    float b = 0.5;
    for(int i=0;i<h;i++)
    {
        for(int j=0;j<w;j++)
        {
            int index = i*step+j;
            float dif;
            if(index<num+1)
                dif = data[index];
            else
                dif = data[index]-data[index-num-1];

            dif *= 1/n;
            m_now = m_pre + dif;
            m_pre = m_now;

            if(data[index]>cvRound(b*m_now))
                dstdata[index] = 255;
            else
                dstdata[index] = 0;
        }
    }
    imshow("dst", dst);
    waitKey(0);
}
vector<int> MultiThreshold(Mat src){
    const int GrayScale = 256;
//    Mat src = imread("/home/top/project/ImageSegmentation/imageSegmentation/Picture1.png", IMREAD_GRAYSCALE);
    int width = src.rows;
    int height = src.cols;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, scale, pixelSum = width * height;
    vector<int> threshold;
    uchar* data = src.ptr<uchar>(0);  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++) //初始化计数数组和比例数组
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
        }
    }

    //计算每个像素在整幅图像中的比例
    float maxPro = 0.0;
    int kk = 0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        if (pixelPro[i] > maxPro)
        {
            maxPro = pixelPro[i];
            kk = i;
        }
    }

    //遍历灰度级[0,255]
    float w0, w1, w2, u0tmp, u1tmp, u2tmp, u0, u1, u2, u, deltaTmp, deltaMax = 0;
    /**
     * w0:属于1景的像素点数占整幅图像的比例
     * w1:属于2景的像素点数占整幅图像的比例
     * w2:属于3景的像素点数占整幅图像的比例
     * u0:1景平均灰度,Σ(当前灰度*此灰度出现的概率)/前景像素点数比例
     * u1:2景平均灰度,Σ(当前灰度*此灰度出现的概率)/背景像素点数比例
     * u2:3景平均灰度,Σ(当前灰度*此灰度出现的概率)/背景像素点数比例
     * u:图像的总平均灰度
     * deltaTmp:遍历阈值时求得的间类方差
     * deltaMax:目前所得的最大间类方差,随着遍历可能会变
     */
    for (i = 0; i < GrayScale; i++)     // i作为阈值
    {
        for (j = 0; j<i-10; j++) {
            w0 = w1 = w2 = u0tmp = u1tmp = u2tmp = u0 = u1 = u2 = u = deltaTmp = 0;
            for (scale = 0; scale < GrayScale; scale++) //遍历每个灰度值，把他们分成前景和背景
            {
                if (scale <= j) {   //1景部分
                    w0 += pixelPro[scale];
                    u0tmp += scale * pixelPro[scale];
                } else if(scale >j && scale <i){ //2景部分
                    w1 += pixelPro[scale];
                    u1tmp += scale * pixelPro[scale];
                }
                else if(scale >= i){
                    w2 += pixelPro[scale];
                    u2tmp += scale * pixelPro[scale];
                }
            }
            u0 = u0tmp / w0;
            u1 = u1tmp / w1;
            u2 = u2tmp / w2;
            u = u0tmp + u1tmp + u2tmp;
            deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2) + w2 * pow((u2 - u), 2);
            if (deltaTmp > deltaMax)
            {
                deltaMax = deltaTmp;
                threshold.clear();
                threshold.push_back(i);
                threshold.push_back(j);
            }
        }

    }
    Mat dst(src.size(), src.type());
    for (int i = 0; i < dst.rows; ++i) {
        for (int j = 0; j < dst.cols; ++j) {
            int value = src.at<uchar>(i, j);
            if (value <= threshold.at(1)) dst.at<uchar>(i, j) = 255;
            else if (value>threshold.at(1) && value<=threshold.at(0)) dst.at<uchar>(i,j) = 128;
            else dst.at<uchar>(i, j) = 0;
        }
    }
    imshow("dst", dst);
    waitKey(0);
    return threshold;
}