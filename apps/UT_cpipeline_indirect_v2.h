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


static double timer()
{
    return getTickCount() / getTickFrequency();
}


int main() {
    string map_file = "E:/projects/MapMatching/data/map_lq/map.jpg";
    string video_file = "E:/projects/MapMatching/data/map_lq/test_imgs/video.mp4";
    Mat cv_map = imread(map_file, IMREAD_COLOR);
    //resize(cv_map, cv_map, Size(), 0.8, 0.8);


    int max_map_len = 1500;
    int max_image_len = 500;
    CVCMat cvc_map = &cv_map;
    CVCMat _image_map_polygon = CVCMatCreate();
    CVCMat _map_map_polygon = CVCMatCreate();

    IMGRGLOC_Image2MapIndirect* image2map_pipe = 0;
    IMGRGLOC_Image2MapIndirect_create(&image2map_pipe);
    IMGRGLOC_Image2MapIndirect_set_map_maxlen(image2map_pipe, 1200);
    IMGRGLOC_Image2MapIndirect_set_ref_image_maxlen(image2map_pipe, 600);
    IMGRGLOC_Image2MapIndirect_set_query_image_maxlen(image2map_pipe, -1);

    IMGRGLOC_Image2MapIndirect_set_map(image2map_pipe, cvc_map);
    IMGRGLOC_Image2MapIndirect_do_map_preprocess(image2map_pipe);


    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE

    VideoCapture cap(video_file);
    // check if we succeeded
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
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        Mat cv_image = frame.clone();
        //resize(cv_image, cv_image, Size(), 0.5, 0.5);
        Mat input_img = cv_image.clone();
        CVCMat cvc_image = &input_img;

        if (count%3==0)
        {
            double match_start = timer();
            IMGRGLOC_Image2MapIndirect_set_ref_image(image2map_pipe, cvc_image);
            IMGRGLOC_Image2MapIndirect_do_ref_image_keypoint_detection(image2map_pipe);
            IMGRGLOC_Image2MapIndirect_do_ref_image_feature_extraction(image2map_pipe);
            bool ref_match_success;
            IMGRGLOC_Image2MapIndirect_do_match_ref2map(image2map_pipe, &ref_match_success);
            double match_end = timer();
            cout << "image2map time: " << fixed << setprecision(2) << (match_end - match_start) * 1000 << " ms" << endl;
        }

        double all_start = timer();
        IMGRGLOC_Image2MapIndirect_set_query_image(image2map_pipe, cvc_image);
        double detect_start = timer();
        IMGRGLOC_Image2MapIndirect_do_query_image_keypoint_detection(image2map_pipe);
        IMGRGLOC_Image2MapIndirect_do_query_image_feature_extraction(image2map_pipe);
        double detect_end = timer();
        bool match_success;

        double match_start = timer();
        IMGRGLOC_Image2MapIndirect_do_match_image2map(image2map_pipe, &match_success);
        double match_end = timer();
        double all_end = timer();

        cout << "detect execution time: " << fixed << setprecision(2) << (detect_end - detect_start) * 1000 << " ms" << endl;
        cout << "match execution time: " << fixed << setprecision(2) << (match_end - match_start) * 1000 << " ms" << endl;
        cout << "total execution time: " << fixed << setprecision(2) << (all_end - all_start) * 1000 << " ms" << endl;
        sum_detect_cost += detect_end - detect_start;
        sum_execu_cost += all_end - all_start;
        sum_match_cost += match_end - match_start;
        float avg_execu_time = sum_execu_cost / (count + 1) * 1000;
        Mat cv_map_show = cv_map.clone();
        if (match_success)
        {
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

        count++;
        //cv::Point pos(20, 50);
        //cv::putText(cv_map_show, to_string(avg_execu_time) + "ms", pos, FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2);
        //string save_filename = "E:/projects/MapMatching/data/output/orb/" + to_string(count) + ".jpg";
        //imwrite(save_filename, cv_map_show);
        resize(cv_map_show, cv_map_show, Size(), 0.7, 0.7);
        //resize(cv_image, cv_image,Size(), 0.6,0.6);
        imshow("map", cv_map_show);
        imshow("image", cv_image);
        if (waitKey(5) >= 0)
            break;
    }
    cv::destroyAllWindows();
    cout << "avg detect execution time per frame: " << fixed << setprecision(2) << sum_detect_cost / (count + 1) * 1000 << " ms" << endl;
    cout << "avg match execution time per frame: " << fixed << setprecision(2) << sum_match_cost / (count + 1) * 1000 << " ms" << endl;
    cout << "avg total execution time  per frame: " << fixed << setprecision(2) << sum_execu_cost / (count + 1) * 1000 << " ms" << endl;
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;


}
