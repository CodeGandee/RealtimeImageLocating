#include "opencvc/CVCTypes.h"
#include "opencvc/CVCImgCodecs.h"
#include "opencv2/opencv.hpp"
#include "IMGRGLOC_Image2Map.h"
#include "IMGRGLOC_Image2MapIndirect.h"
#include "spdlog/spdlog.h"
#include <string>

#define MATCH_INDIRECT

std::string fn_image = R"(D:\code\ImageRegionSearch\data\map\largemap.jpg)";
std::string fn_query_image = R"(D:\code\ImageRegionSearch\data\map\test_imgs\001.jpg)";

#ifdef MATCH_INDIRECT
int main() {
	auto img = cv::imread(fn_image, cv::IMREAD_ANYCOLOR);
	auto query = cv::imread(fn_query_image, cv::IMREAD_ANYCOLOR);

	IMGRGLOC_Image2MapIndirect* obj;
	IMGRGLOC_Image2MapIndirect_create(&obj);

	IMGRGLOC_Image2MapIndirect_set_map(obj, &img);
	IMGRGLOC_Image2MapIndirect_do_map_preprocess(obj);

	if (0)
	{
		cv::Mat ptsmat;
		IMGRGLOC_Image2MapIndirect_get_map_keypoints(obj, &ptsmat);
		std::cout << ptsmat << std::endl;
	}

	IMGRGLOC_Image2MapIndirect_set_ref_image(obj, &query);
	IMGRGLOC_Image2MapIndirect_do_ref_image_keypoint_detection(obj);
	IMGRGLOC_Image2MapIndirect_do_ref_image_feature_extraction(obj);

	IMGRGLOC_Image2MapIndirect_set_query_image(obj, &query);
	IMGRGLOC_Image2MapIndirect_do_query_image_keypoint_detection(obj);
	IMGRGLOC_Image2MapIndirect_do_query_image_feature_extraction(obj);

	{
		bool ok = false;
		//IMGRGLOC_Image2MapIndirect_do_match_ref2map(obj, &ok);
		//spdlog::info("ref2map = {}", ok);

		IMGRGLOC_Image2MapIndirect_do_match_image2map(obj, &ok);
		spdlog::info("query2ref = {}", ok);
	}


	{
		cv::Mat transmat;
		IMGRGLOC_Image2MapIndirect_get_transform_matrix(obj, &transmat);
		spdlog::info("transform matrix");
		std::cout << transmat << std::endl;
		spdlog::info("");
	}

	return 0;
}
#endif

#ifdef MATCH_DIRECT
int main() {
	auto img = cv::imread(fn_image, cv::IMREAD_ANYCOLOR);
	auto query = cv::imread(fn_query_image, cv::IMREAD_ANYCOLOR);
	query.setTo(0);

	IMGRGLOC_Image2Map* obj;
	IMGRGLOC_Image2Map_create(&obj);

	IMGRGLOC_Image2Map_set_map(obj, &img);
	IMGRGLOC_Image2Map_do_map_preprocess(obj);

	if(0)
	{
		cv::Mat ptsmat;
		IMGRGLOC_Image2Map_get_map_keypoints(obj, &ptsmat);
		std::cout << ptsmat << std::endl;
	}

	IMGRGLOC_Image2Map_set_query_image(obj, &query);
	IMGRGLOC_Image2Map_do_query_image_keypoint_detection(obj);
	IMGRGLOC_Image2Map_do_query_image_feature_extraction(obj);

	{
		bool ok = false;
		IMGRGLOC_Image2Map_do_match_image2map(obj, &ok);
	}
	

	{
		cv::Mat pts_query, pts_map;
		IMGRGLOC_Image2Map_get_matched_points_in_image(obj, &pts_query);
		IMGRGLOC_Image2Map_get_matched_points_in_map(obj, &pts_map);

		for (int i = 0; i < pts_query.rows && i<10; i++) {
			auto x0 = pts_query.at<float>(i, 0);
			auto y0 = pts_query.at<float>(i, 1);
			auto x1 = pts_map.at<float>(i, 0);
			auto y1 = pts_map.at<float>(i, 1);
			spdlog::info("({},{})->({},{})", x0, y0, x1, y1);
		}
	}
	
	return 0;
}
#endif