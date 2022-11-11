#include <iostream>
#include <time.h>
#include <opencv2/highgui.hpp>
#include "pipeline/Image2Map.h"

using namespace imgregionloc;
using namespace cv;
using namespace std;

int main() {
    Mat map_img_ori = imread("E:/projects/MapMatching/data/map/map.jpg");
    Mat loc_img_ori = imread("E:/projects/MapMatching/data/map/test_imgs/01.jpg");
    int scale_factor = 8;
    resize(map_img_ori, map_img_ori, Size(map_img_ori.cols / scale_factor, map_img_ori.rows / scale_factor));
    resize(loc_img_ori, loc_img_ori, Size(loc_img_ori.cols / scale_factor, loc_img_ori.rows / scale_factor));
    int mw = map_img_ori.cols;
    int mh = map_img_ori.rows;
    int rw = loc_img_ori.cols;
    int rh = loc_img_ori.rows;



    Mat map_img, loc_img;
    Mat res_region_in_map, res_map_to_region, res_corners_in_map;
    cvtColor(map_img_ori, map_img, COLOR_BGR2GRAY);
    cvtColor(loc_img_ori, loc_img, COLOR_BGR2GRAY);


    clock_t start, end;
    pipeline::Image2Map image2map_pipe;
    image2map_pipe.set_map(map_img);
    image2map_pipe.do_map_preprocess();
    cout << "map ready: " << image2map_pipe.is_map_ready() << endl;

    image2map_pipe.set_query_image(loc_img);
    image2map_pipe.do_query_image_keypoint_detection();
    image2map_pipe.do_query_image_feature_extraction();
    cout << "query image ready: " << image2map_pipe.is_query_image_ready() << endl;
    bool match_success;
    match_success = image2map_pipe.do_match_image2map();
    cout << "match success: " << match_success << endl;
    cout << "match confidence: " << image2map_pipe.get_match_confidence() << endl;
    Point2 anchor;
    //Mat* mask;
    cv::Mat t_matrix;
    t_matrix = image2map_pipe.get_transform_matrix();
    cout << "t_matrix" << t_matrix << endl;

    image2map_pipe.get_transformed_image_in_map(res_region_in_map, anchor);
    cout << "match confidence: " << image2map_pipe.get_match_confidence() << endl;

    imshow("res_region_in_map", res_region_in_map);
    //imshow("mask", mask);

    Polygon2 corners(4);
    corners[0] = Point2(0, 0);
    corners[1] = Point2(rw, 0);
    corners[2] = Point2(rw, rh);
    corners[3] = Point2(0, rh);

    image2map_pipe.transform_point_image2map(corners, corners);
    res_corners_in_map = map_img.clone();
    for (size_t i = 0; i < corners.size(); i++)
    {
        if (i > 2)
        {
            line(res_corners_in_map, corners[i], corners[0], Scalar(0, 0, 255), 2);
        }
        else {
            line(res_corners_in_map, corners[i], corners[i + 1], Scalar(0, 0, 255), 2);
        }
        circle(res_corners_in_map, corners[i], 4, Scalar(255, 0, 0), -1);

    }

    Polygon2 map_region_in_image;
    image2map_pipe.get_map_region_wrt_image(map_region_in_image);
    for (size_t i = 0; i < map_region_in_image.size(); i++)
    {
        if (i > map_region_in_image.size() - 2)
        {
            line(loc_img_ori, map_region_in_image[i], map_region_in_image[0], Scalar(0, 0, 255), 2);
        }
        else {
            line(loc_img_ori, map_region_in_image[i], map_region_in_image[i + 1], Scalar(0, 0, 255), 2);
        }
        circle(loc_img_ori, map_region_in_image[i], 4, Scalar(255, 0, 0), -1);

    }


    Polygon2 image_region_in_map;
    image2map_pipe.get_map_region_wrt_map(image_region_in_map);
    for (size_t i = 0; i < image_region_in_map.size(); i++)
    {
        if (i > image_region_in_map.size() - 2)
        {
            line(map_img_ori, image_region_in_map[i], image_region_in_map[0], Scalar(0, 0, 255), 2);
        }
        else {
            line(map_img_ori, image_region_in_map[i], image_region_in_map[i + 1], Scalar(0, 0, 255), 2);
        }
        circle(map_img_ori, image_region_in_map[i], 4, Scalar(255, 0, 0), -1);

    }

    imshow("loc_img_ori", loc_img_ori);
    imshow("map_img_ori", map_img_ori);
    imshow("res_corners_in_map", res_corners_in_map);
    waitKey();
    destroyAllWindows();
    return 0;
}