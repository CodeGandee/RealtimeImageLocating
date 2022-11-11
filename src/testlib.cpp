#include <iostream>
#include "testlib.h"
#include "opencv2/opencv.hpp"
#include "pipeline/Image2MapBase.h"
#include "pipeline/Image2MapIndirect.h"

cv::Mat make_random_image(){
    cv::Mat x(100,200, CV_8UC1);
    cv::randu(x,0,255);
    return x;
}