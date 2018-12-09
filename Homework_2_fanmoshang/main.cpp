#include "ThresholdMethods.h"
#include "WatershedMethod.h"

using namespace std;
using namespace cv;

int main() {
//    threshold_myself(50);
//    auto_set_threshold();
//    int threshold = otsuMethodDemo();
//    my_localAdaptiveThresholding();
//    src_adaptiveThreshold();
//    blockedThresholdSegmentation();
//    averMovingSegmentation();


    Mat src = imread("/home/top/project/ImageSegmentation/imageSegmentation/Picture6.png", IMREAD_GRAYSCALE);
    vector<int> threshold;
    threshold = MultiThreshold(src);
    cout << "threshold0:"<< threshold.at(0)<<endl;
    cout << "threshold1:"<< threshold.at(1)<<endl;

    return 0;
}