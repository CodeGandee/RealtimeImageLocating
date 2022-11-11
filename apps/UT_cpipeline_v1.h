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
#include "pipeline/Image2MapDirect.h"

using Image2Map = imgregionloc::pipeline::Image2MapDirect;

using namespace cv;
using namespace std;
int main() {
	Mat map_img_ori = imread("E:/projects/MapMatching/data/map/map.jpg");
	Mat loc_img_ori = imread("E:/projects/MapMatching/data/map/test_imgs/01.jpg");
	int scale_factor_img = 6;
	int scale_factor_map = 2;
	//resize(map_img_ori, map_img_ori, Size(map_img_ori.cols / scale_factor_map, map_img_ori.rows / scale_factor_map));
	//resize(loc_img_ori, loc_img_ori, Size(loc_img_ori.cols / scale_factor_img, loc_img_ori.rows / scale_factor_img));
	//cvtColor(map_img_ori, map_img_ori, COLOR_BGR2GRAY);
	//cvtColor(loc_img_ori, loc_img_ori, COLOR_BGR2GRAY);
	cv::Mat t_matrix(3, 3, CV_32F);
	cv::Mat matched_pts_in_map;
	cv::Mat matched_pts_in_image;
	cv::Mat transformed_image;
	cv::Mat out_mask;
	cv::Mat image_map_polygon;
	cv::Mat map_map_polygon;
	cv::Mat test_pt1, test_pts1, test_pt2, test_pts2;
	cv::Mat test_pt = (Mat_<float>(1, 2) << 0, 0);
	//cv::Mat test_pt = (Mat_<float>(1, 2) << 250, 250);
	cv::Mat test_pts = (Mat_<float>(4, 2) << 10, 10, 60, 10, 60, 60, 10, 60);
	//cv::Mat test_pts = (Mat_<float>(4, 2) << 300, 200, 400, 200, 400, 300, 300, 300);
	clock_t start, end;


	CVCMat m_img = CVCMatCreateWithSize(map_img_ori.rows, map_img_ori.cols, map_img_ori.type(), (char*)map_img_ori.data);

	//CVCMat m_img = &map_img_ori;
	//CVCMat q_img = CVCMatCreateWithSize(loc_img_ori.rows, loc_img_ori.cols, loc_img_ori.type(), (char*)loc_img_ori.data);
	CVCMat q_img = CVCMatCreate();
	CVCMatCreateData(q_img, loc_img_ori.rows, loc_img_ori.cols, loc_img_ori.type());
	CVCMatCopyFrom(q_img, (char*)loc_img_ori.data);
	//CVCMat q_img = &loc_img_ori;
	CVCMat transform_matrix = &t_matrix;
	//CVCMat transform_matrix = &t_matrix;
	CVCMat _matched_pts_in_map = &matched_pts_in_map;
	CVCMat _matched_pts_in_image = &matched_pts_in_image;
	CVCMat _transformed_image = &transformed_image;
	CVCMat _out_mask = &out_mask;
	CVCMat _image_map_polygon = &image_map_polygon;
	CVCMat _map_map_polygon = &map_map_polygon;
	CVCMat _test_pt = &test_pt;
	CVCMat _test_pts = &test_pts;
	float x2 = 200;
	float x3 = 300;
	float x4 = 400;
	CVCMatSetFloat(_test_pts, 0, 0, x3);
	CVCMatSetFloat(_test_pts, 0, 1, x2);
	CVCMatSetFloat(_test_pts, 1, 0, x4);
	CVCMatSetFloat(_test_pts, 1, 1, x2);
	CVCMatSetFloat(_test_pts, 2, 0, x4);
	CVCMatSetFloat(_test_pts, 2, 1, x3);
	CVCMatSetFloat(_test_pts, 3, 0, x3);
	CVCMatSetFloat(_test_pts, 3, 1, x3);
	cout << *(CVCMatAtFloat(_test_pts, 0, 0)) << endl;
	cout << *(CVCMatAtFloat(_test_pts, 0, 1)) << endl;
	cout << *(CVCMatAtFloat(_test_pts, 1, 0)) << endl;
	cout << *(CVCMatAtFloat(_test_pts, 1, 1)) << endl;
	cout << *(CVCMatAtFloat(_test_pts, 2, 0)) << endl;

	CVCMat _test_pt1 = &test_pt1;
	CVCMat _test_pts1 = &test_pts1;
	CVCMat _test_pt2 = &test_pt2;
	CVCMat _test_pts2 = &test_pts2;

	//CVCMat img2 = CVCMatCreate();
	//map_img_ori.copyTo(*(cv::Mat*)img2);
	//CVCMatFree(img2);



	IMGRGLOC_Image2Map* image2map_pipe = 0;
	IMGRGLOC_Image2Map_create(&image2map_pipe);
	IMGRGLOC_Image2Map_set_map(image2map_pipe, m_img);
	IMGRGLOC_Image2Map_set_query_image(image2map_pipe, q_img);
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
	IMGRGLOC_Image2Map_get_match_confidence(image2map_pipe, &confidence);
	IMGRGLOC_Image2Map_get_matched_points_in_map(image2map_pipe, _matched_pts_in_map);
	IMGRGLOC_Image2Map_get_matched_points_in_image(image2map_pipe, _matched_pts_in_image);
	IMGRGLOC_Image2Map_get_transformed_image_in_map(image2map_pipe, _transformed_image, &tlx, &tly, _out_mask);
	IMGRGLOC_Image2Map_get_map_region_wrt_image(image2map_pipe, _image_map_polygon);
	IMGRGLOC_Image2Map_get_map_region_wrt_map(image2map_pipe, _map_map_polygon);
	IMGRGLOC_Image2Map_transform_point_image2map(image2map_pipe, _test_pt, _test_pt1);
	IMGRGLOC_Image2Map_transform_points_image2map(image2map_pipe, _test_pts, _test_pts1);
	IMGRGLOC_Image2Map_transform_point_map2image(image2map_pipe, _test_pt, _test_pt2);
	IMGRGLOC_Image2Map_transform_points_map2image(image2map_pipe, _test_pts, _test_pts2);

	CVCMatAt;

	vector<Point2f> image_map_region;
	vector<Point2f> map_map_region;
	vector<Point2f> test_pts_in_vector, out_test_pt1, out_test_pts1, out_test_pt2, out_test_pts2;

	for (int i = 0; i < test_pts.rows; i++)
	{
		Point2f tmp;
		tmp.x = test_pts.at<float>(i, 0);
		tmp.y = test_pts.at<float>(i, 1);
		test_pts_in_vector.push_back(tmp);
	}
	for (int i = 0; i < test_pt1.rows; i++)
	{
		Point2f tmp;
		tmp.x = test_pt1.at<float>(i, 0);
		tmp.y = test_pt1.at<float>(i, 1);
		out_test_pt1.push_back(tmp);
	}
	for (int i = 0; i < test_pts1.rows; i++)
	{
		Point2f tmp;
		tmp.x = test_pts1.at<float>(i, 0);
		tmp.y = test_pts1.at<float>(i, 1);
		out_test_pts1.push_back(tmp);
	}
	for (int i = 0; i < test_pt2.rows; i++)
	{
		Point2f tmp;
		tmp.x = test_pt2.at<float>(i, 0);
		tmp.y = test_pt2.at<float>(i, 1);
		out_test_pt2.push_back(tmp);
	}
	for (int i = 0; i < test_pts2.rows; i++)
	{
		Point2f tmp;
		tmp.x = test_pts2.at<float>(i, 0);
		tmp.y = test_pts2.at<float>(i, 1);
		out_test_pts2.push_back(tmp);
	}
	for (int i = 0; i < image_map_polygon.rows; i++)
	{
		Point2f tmp;
		tmp.x = image_map_polygon.at<float>(i, 0);
		tmp.y = image_map_polygon.at<float>(i, 1);
		image_map_region.push_back(tmp);
	}
	cout << image_map_polygon << endl;
	for (int i = 0; i < map_map_polygon.rows; i++)
	{
		Point2f tmp;
		tmp.x = map_map_polygon.at<float>(i, 0);
		tmp.y = map_map_polygon.at<float>(i, 1);
		map_map_region.push_back(tmp);
	}
	cout << map_map_polygon << endl;

	{
		auto _image2map_pipe = reinterpret_cast<Image2Map*>(image2map_pipe);
		cout << "is query image ready: " << _image2map_pipe->is_query_image_ready() << endl;
		cout << "is map ready: " << _image2map_pipe->is_map_ready() << endl;
		cout << "match success: " << match_success << endl;
		cout << "transform_matrix: " << t_matrix << endl;
		//cout << "matched_pts_map: " << matched_pts_in_map << endl;
		//cout << "matched_pts_image: " << matched_pts_in_image << endl;

		for (size_t i = 0; i < image_map_region.size(); i++)
		{
			if (i > 2)
			{
				line(loc_img_ori, image_map_region[i], image_map_region[0], Scalar(0, 0, 255), 2);
			}
			else {
				line(loc_img_ori, image_map_region[i], image_map_region[i + 1], Scalar(0, 0, 255), 2);
			}
			circle(loc_img_ori, image_map_region[i], 4, Scalar(255, 0, 0), -1);

		}
		for (size_t i = 0; i < map_map_region.size(); i++)
		{
			if (i > 2)
			{
				line(map_img_ori, map_map_region[i], map_map_region[0], Scalar(0, 0, 255), 2);
			}
			else {
				line(map_img_ori, map_map_region[i], map_map_region[i + 1], Scalar(0, 0, 255), 2);
			}
			circle(map_img_ori, map_map_region[i], 4, Scalar(255, 0, 0), -1);

		}


		circle(map_img_ori, out_test_pt1[0], 4, Scalar(255, 0, 0), -1);
		circle(loc_img_ori, out_test_pt2[0], 4, Scalar(255, 0, 0), -1);

		for (size_t i = 0; i < out_test_pts1.size(); i++)
		{
			circle(map_img_ori, out_test_pts1[i], 4, Scalar(0, 0, 0), -1);
			circle(loc_img_ori, test_pts_in_vector[i], 4, Scalar(0, 0, 0), -1);
		}

		for (size_t i = 0; i < out_test_pts2.size(); i++)
		{
			circle(loc_img_ori, out_test_pts2[i], 4, Scalar(255, 0, 0), -1);
			circle(map_img_ori, test_pts_in_vector[i], 4, Scalar(255, 0, 0), -1);
		}

		cv::imshow("out_mask", out_mask);
		cv::imshow("transformed_image", transformed_image);
		cv::imshow("loc_img_ori", loc_img_ori);
		cv::imshow("map_img_ori", map_img_ori);
		cv::waitKey();
		cv::destroyAllWindows();

	}

	IMGRGLOC_Image2Map_destroy(image2map_pipe);
	return 0;
}