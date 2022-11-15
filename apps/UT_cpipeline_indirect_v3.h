#pragma once

#include "IMGRGLOC_Image2MapIndirect.h"
#include <memory>
#include <windows.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include "pipeline/Image2MapIndirect.h"

using Image2MapIndirect = imgregionloc::pipeline::Image2MapIndirect;

using namespace cv;
using namespace std;

std::mutex cap_lock;


static double timer()
{
    return getTickCount() / getTickFrequency();
}


int directMatch(IMGRGLOC_Image2MapIndirect* image2map_pipe,  VideoCapture* cap)
{
    while (1)
    {
        Mat frame;
        {
            std::lock_guard<std::mutex> lock(cap_lock);
            cap->read(frame);
            //rotate(frame, frame, ROTATE_180);
            if (frame.empty()) {
                return 0;
            }
        }

        Mat cv_image = frame.clone();
        CVCMat cvc_image = &cv_image;
        IMGRGLOC_Image2MapIndirect_set_ref_image(image2map_pipe, cvc_image);
        IMGRGLOC_Image2MapIndirect_do_ref_image_keypoint_detection(image2map_pipe);
        IMGRGLOC_Image2MapIndirect_do_ref_image_feature_extraction(image2map_pipe);
        bool ref_match_success;
        IMGRGLOC_Image2MapIndirect_do_match_ref2map(image2map_pipe, &ref_match_success);
        
    }
}

int indirectMatchAndShow(IMGRGLOC_Image2MapIndirect* image2map_pipe, Mat cv_map, VideoCapture* cap)
{
    int count = 0;

    while (1)
    {
        Mat frame;
        {
            std::lock_guard<std::mutex> lock(cap_lock);
            cap->read(frame);
            //rotate(frame, frame, ROTATE_180);
            if (frame.empty()) {
                return 0;
            }
        }
        Mat cv_image = frame.clone();
        CVCMat cvc_image = &cv_image;
        CVCMat cvc_map = &cv_map;
        CVCMat _image_map_polygon = CVCMatCreate();
        CVCMat _map_map_polygon = CVCMatCreate();
        IMGRGLOC_Image2MapIndirect_set_query_image(image2map_pipe, cvc_image);
        IMGRGLOC_Image2MapIndirect_do_query_image_keypoint_detection(image2map_pipe);
        IMGRGLOC_Image2MapIndirect_do_query_image_feature_extraction(image2map_pipe);
        bool match_success;

        IMGRGLOC_Image2MapIndirect_do_match_image2map(image2map_pipe, &match_success);
        count++;
        Mat cv_map_show = cv_map.clone();
        if (match_success)
        {
            cout << count << endl;
            IMGRGLOC_Image2MapIndirect_get_map_region_wrt_image(image2map_pipe, _image_map_polygon);
            IMGRGLOC_Image2MapIndirect_get_map_region_wrt_map(image2map_pipe, _map_map_polygon);

            for (size_t i = 0; i < CVCMatHeight(_image_map_polygon); i++)
            {
                Point2f pt1;
                Point2f pt2;
                if (i > CVCMatHeight(_image_map_polygon) - 2)
                {
                    pt1 = Point2f(*CVCMatAtFloat(_image_map_polygon, i, 0), *CVCMatAtFloat(_image_map_polygon, i, 1));
                    pt2 = Point2f(*CVCMatAtFloat(_image_map_polygon, 0, 0), *CVCMatAtFloat(_image_map_polygon, 0, 1));
                }
                else {
                    pt1 = Point2f(*CVCMatAtFloat(_image_map_polygon, i, 0), *CVCMatAtFloat(_image_map_polygon, i, 1));
                    pt2 = Point2f(*CVCMatAtFloat(_image_map_polygon, i + 1, 0), *CVCMatAtFloat(_image_map_polygon, i + 1, 1));

                }
                line(cv_image, pt1, pt2, Scalar(0, 255, 0), 2);
                circle(cv_image, pt1, 4, Scalar(128, 128, 0), -1);
            }

            for (size_t i = 0; i < CVCMatHeight(_map_map_polygon); i++)
            {
                Point2f pt1;
                Point2f pt2;
                if (i > CVCMatHeight(_map_map_polygon) - 2)
                {
                    pt1 = Point2f(*CVCMatAtFloat(_map_map_polygon, i, 0), *CVCMatAtFloat(_map_map_polygon, i, 1));
                    pt2 = Point2f(*CVCMatAtFloat(_map_map_polygon, 0, 0), *CVCMatAtFloat(_map_map_polygon, 0, 1));
                }
                else {
                    pt1 = Point2f(*CVCMatAtFloat(_map_map_polygon, i, 0), *CVCMatAtFloat(_map_map_polygon, i, 1));
                    pt2 = Point2f(*CVCMatAtFloat(_map_map_polygon, i + 1, 0), *CVCMatAtFloat(_map_map_polygon, i + 1, 1));

                }
                line(cv_map_show, pt1, pt2, Scalar(0, 255, 0), 2);
                circle(cv_map_show, pt1, 4, Scalar(128, 128, 0), -1);
            }
        }

        cv::resize(cv_map_show, cv_map_show, Size(), 0.7, 0.7);
        cv::resize(cv_image, cv_image, Size(), 0.5, 0.5);
        cv::imshow("map", cv_map_show);
        cv::imshow("image", cv_image);
        if (waitKey(5) >= 0) {
            return 1;
        }

    };
}

int main() {
    string map_file = "E:/projects/MapMatching/data/map_lq/map.jpg";
    string video_file = "E:/projects/MapMatching/data/map_lq/test_imgs/video4.mp4";
    Mat cv_map = imread(map_file, IMREAD_COLOR);
    //resize(cv_map, cv_map, Size(), 0.8, 0.8);


    int max_map_len = 1500;
    int max_image_len = 500;
    CVCMat cvc_map = &cv_map;
    CVCMat _image_map_polygon = CVCMatCreate();
    CVCMat _map_map_polygon = CVCMatCreate();

    IMGRGLOC_Image2MapIndirect* image2map_pipe = 0;
    IMGRGLOC_Image2MapIndirect_create(&image2map_pipe);
    IMGRGLOC_Image2MapIndirect_set_map_maxlen(image2map_pipe, 1000);
    IMGRGLOC_Image2MapIndirect_set_ref_image_maxlen(image2map_pipe, 500);
    IMGRGLOC_Image2MapIndirect_set_query_image_maxlen(image2map_pipe, -1);

    IMGRGLOC_Image2MapIndirect_set_map(image2map_pipe, cvc_map);
    IMGRGLOC_Image2MapIndirect_do_map_preprocess(image2map_pipe);


    Mat frame;
    VideoCapture cap(video_file);
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;
    int count = 0;
    double sum_execu_cost = 0;
    double sum_detect_cost = 0;
    double sum_match_cost = 0;
    std::thread t1(directMatch, image2map_pipe, &cap);
    std::thread t2(indirectMatchAndShow, image2map_pipe, cv_map, &cap);
    t1.join();
    t2.join();

    destroyAllWindows();
    return 0;


}
