#pragma once
#include "IMGRGLOC_Image2Map.h"
#include <memory>
#include <time.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <vector>
#include "pipeline/Image2Map.h"

using Image2Map = imgregionloc::pipeline::Image2Map;

using namespace cv;
using namespace std;

int main() {
	string map_file = "E:/projects/MapMatching/data/map_lq/map.jpg";
	//string map_file = "E:/projects/MapMatching/data/map_lq/test_imgs/20.jpg";
	string image_file = "E:/projects/MapMatching/data/map_lq/test_imgs/tmp.jpg";
	//CVCMat cvc_map = CVCMatCreate();
	//CVCMat cvc_image = CVCMatCreate();
	CVCMat cvc_map = CVCimread((char*)map_file.data(), IMREAD_COLOR);
	CVCMat cvc_image = CVCimread((char*)image_file.data(), IMREAD_COLOR);
	//CVCMat cvc_map = 0;
	//CVCMat cvc_image = 0;

	//cv::Mat _cvc_map, _cvc_image;
	//{
	//	_cvc_map = cv::imread(map_file, cv::IMREAD_COLOR);
	//	_cvc_image = cv::imread(image_file, cv::IMREAD_COLOR);

	//	auto h = _cvc_image.rows;
	//	auto w = _cvc_image.cols;
	//	int cutsize = 100;
	//	auto _tmp = _cvc_image(cv::Rect(w / 2 - cutsize / 2, h / 2 - cutsize / 2, cutsize, cutsize));
	//	_cvc_image = _tmp.clone();

	//	cvc_map = &_cvc_map;
	//	cvc_image = &_cvc_image;
	//}

	clock_t start, end;

	CVCMat transform_matrix = CVCMatCreateWithSize(3, 3, CV_32F);
	
	//CVCMat transform_matrix = &t_matrix;
	CVCMat _matched_pts_in_map = CVCMatCreate();
	CVCMat _matched_pts_in_image = CVCMatCreate();
	CVCMat _transformed_image = CVCMatCreate();
	CVCMat _out_mask = CVCMatCreate();
	CVCMat _image_map_polygon = CVCMatCreate();
	CVCMat _map_map_polygon = CVCMatCreate();
	CVCMat _test_pt = CVCMatCreateWithSize(1, 2, CV_32F);
	CVCMat _test_pts = CVCMatCreateWithSize(4, 2, CV_32F);

	CVCMat _out_test_pt_in_map = CVCMatCreate();
	CVCMat _out_test_pts_in_map = CVCMatCreate();
	CVCMat _out_test_pt_in_image = CVCMatCreate();
	CVCMat _out_test_pts_in_image = CVCMatCreate();

	float x1 = 120;
	float x2 = 200;
	float x3 = 300;
	float x4 = 400;
	CVCMatSetFloat(_test_pt, 0, 0, x1);
	CVCMatSetFloat(_test_pt, 0, 1, x1);
	CVCMatSetFloat(_test_pts, 0, 0, x3);
	CVCMatSetFloat(_test_pts, 0, 1, x2);
	CVCMatSetFloat(_test_pts, 1, 0, x4);
	CVCMatSetFloat(_test_pts, 1, 1, x2);
	CVCMatSetFloat(_test_pts, 2, 0, x4);
	CVCMatSetFloat(_test_pts, 2, 1, x3);
	CVCMatSetFloat(_test_pts, 3, 0, x3);
	CVCMatSetFloat(_test_pts, 3, 1, x3);

	if (1)
	{
		Point2f xx(1.3, 4.5);
		Point2f yy = xx * 2.2;
		int a = 1;
	}

	int max_map_len = 2000;
	int max_image_len = 500;
	IMGRGLOC_Image2Map* image2map_pipe = 0;
	IMGRGLOC_Image2Map_create(&image2map_pipe);
	IMGRGLOC_Image2Map_set_map(image2map_pipe, cvc_map, max_map_len);
	IMGRGLOC_Image2Map_set_query_image(image2map_pipe, cvc_image, max_image_len);
	start = clock();
	IMGRGLOC_Image2Map_do_map_preprocess(image2map_pipe);
	end = clock();
	cout << "map preprocess cost: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds" << endl;
	start = clock();
	IMGRGLOC_Image2Map_do_map_preprocess(image2map_pipe);
	end = clock();
	cout << "map preprocess cost: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds" << endl;
	start = clock();
	IMGRGLOC_Image2Map_do_query_image_keypoint_detection(image2map_pipe);
	IMGRGLOC_Image2Map_do_query_image_feature_extraction(image2map_pipe);
	end = clock();
	cout << "image preprocess cost: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds" << endl;
	bool match_success;
	float confidence;
	int tlx, tly;
	start = clock();
	IMGRGLOC_Image2Map_do_match_image2map(image2map_pipe, &match_success);
	end = clock();
	cout << "match cost: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds" << endl;
	IMGRGLOC_Image2Map_get_transform_matrix(image2map_pipe, transform_matrix);
	if(0)
	{
		auto& cv_transform_matrix = *(cv::Mat*)transform_matrix;
		auto& cv_image = *(cv::Mat*)cvc_image;
		CVCMat _test_pt_xx = CVCMatCreate();
		vector<Point2f> pt, pt2, pt3, pt4;
		pt.push_back(Point2f(0, 0));
		pt.push_back(Point2f(0, 1080));
		pt.push_back(Point2f(1840, 1080));
		pt.push_back(Point2f(1840, 0));
		Mat invert_t;
		invert(cv_transform_matrix, invert_t);
		invert_t /= invert_t.at<double>(2, 2);
		cout << "forward transform" << endl;
		cout << cv_transform_matrix << endl;
		cout << "inverse transform" << endl;
		cout << invert_t << endl;
		cv::perspectiveTransform(pt, pt2, invert_t);
		cv::perspectiveTransform(pt2, pt3, cv_transform_matrix);
		cv::perspectiveTransform(pt3, pt4, invert_t);
		cout << pt << endl;
		cout << pt2 << endl;
		cout << pt3 << endl;
		cout << pt4 << endl;
		
		IMGRGLOC_Image2Map_transform_point_image2map(image2map_pipe, _test_pt, _out_test_pt_in_image);
		IMGRGLOC_Image2Map_transform_point_map2image(image2map_pipe, _out_test_pt_in_image, _test_pt_xx);
		
		Point2f xx(*CVCMatAtFloat(_test_pt, 0, 0), *CVCMatAtFloat(_test_pt, 0, 1));
		Point2f xx2(*CVCMatAtFloat(_test_pt_xx, 0, 0), *CVCMatAtFloat(_test_pt_xx, 0, 1));
		circle(cv_image, xx, 4, Scalar(255, 0, 0), -1);
		circle(cv_image, xx2, 4, Scalar(0, 255, 0), -1);
		int aa = 10;
	}


	IMGRGLOC_Image2Map_get_match_confidence(image2map_pipe, &confidence);
	IMGRGLOC_Image2Map_get_matched_points_in_map(image2map_pipe, _matched_pts_in_map);
	IMGRGLOC_Image2Map_get_matched_points_in_image(image2map_pipe, _matched_pts_in_image);
	IMGRGLOC_Image2Map_get_transformed_image_in_map(image2map_pipe, _transformed_image, &tlx, &tly, _out_mask);
	IMGRGLOC_Image2Map_get_map_region_wrt_image(image2map_pipe, _image_map_polygon);
	IMGRGLOC_Image2Map_get_image_region_wrt_map(image2map_pipe, _map_map_polygon);
	IMGRGLOC_Image2Map_transform_point_image2map(image2map_pipe, _test_pt, _out_test_pt_in_map);
	IMGRGLOC_Image2Map_transform_points_image2map(image2map_pipe, _test_pts, _out_test_pts_in_map);
	IMGRGLOC_Image2Map_transform_point_map2image(image2map_pipe, _test_pt, _out_test_pt_in_image);
	IMGRGLOC_Image2Map_transform_points_map2image(image2map_pipe, _test_pts, _out_test_pts_in_image);

	cout << "confidence: "<< confidence << endl;


	{
		auto& cv_map = *(cv::Mat*)cvc_map;
		auto& cv_image = *(cv::Mat*)cvc_image;
		auto& transformed_image = *(cv::Mat*)_transformed_image;
		auto _image2map_pipe = reinterpret_cast<Image2Map*>(image2map_pipe);
		
		for (size_t i = 0; i < CVCMatHeight(_image_map_polygon); i++)
		{
			Point2f pt1;
			Point2f pt2;
			if (i > CVCMatHeight(_image_map_polygon)-2)
			{
				pt1 = Point2f(*CVCMatAtFloat(_image_map_polygon,i,0), *CVCMatAtFloat(_image_map_polygon, i, 1));
				pt2 = Point2f(*CVCMatAtFloat(_image_map_polygon,0,0), *CVCMatAtFloat(_image_map_polygon, 0, 1));
			}
			else {
				pt1 = Point2f(*CVCMatAtFloat(_image_map_polygon, i, 0), *CVCMatAtFloat(_image_map_polygon, i, 1));
				pt2 = Point2f(*CVCMatAtFloat(_image_map_polygon, i+1, 0), *CVCMatAtFloat(_image_map_polygon, i+1, 1));
				
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
			line(cv_map, pt1, pt2, Scalar(0, 255, 0), 2);
			circle(cv_map, pt1, 4, Scalar(128, 128, 0), -1);
		}
		for (size_t i = 0; i < 4; i++)
		{
			Point2f pt1_test;
			Point2f pt2_test;
			Point2f pt1_in_map;
			Point2f pt2_in_map;
			Point2f pt1_in_image;
			Point2f pt2_in_image;
			if (i > CVCMatHeight(_map_map_polygon) - 2)
			{
				pt1_test = Point2f(*CVCMatAtFloat(_test_pts, i, 0), *CVCMatAtFloat(_test_pts, i, 1));
				pt1_test = Point2f(*CVCMatAtFloat(_test_pts, i, 0), *CVCMatAtFloat(_test_pts, i, 1));
				pt1_in_map = Point2f(*CVCMatAtFloat(_out_test_pts_in_map, i, 0), *CVCMatAtFloat(_out_test_pts_in_map, i, 1));
				pt2_in_map = Point2f(*CVCMatAtFloat(_out_test_pts_in_map, 0, 0), *CVCMatAtFloat(_out_test_pts_in_map, 0, 1));
				pt1_in_image = Point2f(*CVCMatAtFloat(_out_test_pts_in_image, i, 0), *CVCMatAtFloat(_out_test_pts_in_image, i, 1));
				pt2_in_image = Point2f(*CVCMatAtFloat(_out_test_pts_in_image, 0, 0), *CVCMatAtFloat(_out_test_pts_in_image, 0, 1));
			}
			else {
				pt1_test = Point2f(*CVCMatAtFloat(_test_pts, i, 0), *CVCMatAtFloat(_test_pts, i, 1));
				pt1_test = Point2f(*CVCMatAtFloat(_test_pts, i, 0), *CVCMatAtFloat(_test_pts, i, 1));
				pt1_in_map = Point2f(*CVCMatAtFloat(_out_test_pts_in_map, i, 0), *CVCMatAtFloat(_out_test_pts_in_map, i, 1));
				pt2_in_map = Point2f(*CVCMatAtFloat(_out_test_pts_in_map, i+1, 0), *CVCMatAtFloat(_out_test_pts_in_map, i+1, 1));
				pt1_in_image = Point2f(*CVCMatAtFloat(_out_test_pts_in_image, i, 0), *CVCMatAtFloat(_out_test_pts_in_image, i, 1));
				pt2_in_image = Point2f(*CVCMatAtFloat(_out_test_pts_in_image, i+1, 0), *CVCMatAtFloat(_out_test_pts_in_image, i+1, 1));

			}
			circle(cv_image, pt1_in_image, 4, Scalar(255, 0, 0), -1);
			circle(cv_map, pt1_in_map, 4, Scalar(0, 0, 255), -1);
			circle(cv_image, pt1_test, 4, Scalar(0, 0, 255), -1);
			circle(cv_map, pt1_test, 4, Scalar(255, 0, 0), -1);
		}
		cv::imshow("transformed_image", transformed_image);
		cv::imshow("image", cv_image);
		cv::imshow("map", cv_map);
		cv::waitKey();
		cv::destroyAllWindows();
	}


	IMGRGLOC_Image2Map_destroy(image2map_pipe);
	return 0;
}