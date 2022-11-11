#include "IMGRGLOC_Image2Map.h"
#include "pipeline/Image2MapDirect.h"
#include "FeaturePointList.h"
#include <string>

namespace rloc = imgregionloc;
using Image2Map = rloc::pipeline::Image2MapDirect;

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_set_map(IMGRGLOC_Image2Map* obj, CVCMat img)
{
	auto _img = (cv::Mat*)img;
	auto p = (Image2Map*)obj;
	p->set_map(*_img);
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_create(IMGRGLOC_Image2Map** obj) {
	auto _obj = new Image2Map();
	*obj = (IMGRGLOC_Image2Map*)_obj;
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_destroy(IMGRGLOC_Image2Map* obj) {
	delete (Image2Map*)obj;
	return IMGRGLOC_ERR_NONE;
}


IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_do_map_preprocess(IMGRGLOC_Image2Map* obj)
{
	auto p = (Image2Map*)obj;
	p->do_map_preprocess();
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_is_map_ready(IMGRGLOC_Image2Map* obj, bool* out_is_ready)
{
	auto p = (Image2Map*)obj;
	*out_is_ready = p->is_map_ready();
	return IMGRGLOC_ERR_NONE;
}


IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_do_query_image_keypoint_detection(IMGRGLOC_Image2Map* obj)
{
	auto p = (Image2Map*)obj;
	p->do_query_image_keypoint_detection();
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_do_query_image_feature_extraction(IMGRGLOC_Image2Map* obj)
{
	auto p = (Image2Map*)obj;
	p->do_query_image_feature_extraction();
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_is_query_image_ready(IMGRGLOC_Image2Map* obj, bool* out_is_ready)
{
	auto p = (Image2Map*)obj;
	*out_is_ready = p->is_query_image_ready();
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_do_match_image2map(IMGRGLOC_Image2Map* obj, bool* out_is_success)
{
	auto p = (Image2Map*)obj;
	*out_is_success = p->do_match_image2map();
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_transform_matrix(IMGRGLOC_Image2Map* obj, CVCMat out_transform_matrix)
{
	auto& _transform_matrix = *(cv::Mat*)out_transform_matrix;
	auto p = (Image2Map*)obj;
	_transform_matrix = p->get_transform_matrix();
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_match_confidence(IMGRGLOC_Image2Map* obj, float* out_confidence)
{
	auto p = (Image2Map*)obj;
	*out_confidence = p->get_match_confidence();
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_matched_points_in_image(IMGRGLOC_Image2Map* obj, CVCMat out_points)
{
	auto p = (Image2Map*)obj;
	rloc::PointList2 pts;
	p->get_matched_points_in_image(pts);
	cv::Mat pts_mat(pts.size(), 2, CV_32F);
	auto& _out_points = *(cv::Mat*)out_points;

	for (int i = 0; i < pts_mat.rows; i++)
	{
		pts_mat.at<float>(i, 0) = pts[i].x;
		pts_mat.at<float>(i, 1) = pts[i].y;
	}
	_out_points = pts_mat;

	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_transform_point_image2map(IMGRGLOC_Image2Map* obj, CVCMat in_point, CVCMat out_point)
{
	auto p = (Image2Map*)obj;
	auto& _in_point = *(cv::Mat*)in_point;
	auto& _out_point = *(cv::Mat*)out_point;

	rloc::Point2 in_pt, out_pt;
	in_pt.x = _in_point.at<float>(0, 0);
	in_pt.y = _in_point.at<float>(0, 1);

	out_pt = p->transform_point_image2map(in_pt);

	cv::Mat pt_mat(1, 2, CV_32F);
	pt_mat.at<float>(0, 0) = out_pt.x;
	pt_mat.at<float>(0, 1) = out_pt.y;

	_out_point = pt_mat;

	return IMGRGLOC_ERR_NONE;

}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_transform_points_image2map(IMGRGLOC_Image2Map* obj, CVCMat in_points, CVCMat out_points)
{
	auto p = (Image2Map*)obj;
	auto& _out_points = *(cv::Mat*)out_points;
	auto& _in_points = *(cv::Mat*)in_points;

	rloc::PointList2 in_pts;
	rloc::PointList2 out_pts;

	for (int i = 0; i < _in_points.rows; i++)
	{
		rloc::Point2 tmp;
		tmp.x = _in_points.at<float>(i, 0);
		tmp.y = _in_points.at<float>(i, 1);
		in_pts.push_back(tmp);
	}
	p->transform_point_image2map(in_pts, out_pts);
	cv::Mat out_mat(out_pts.size(), 2, CV_32F);
	for (int i = 0; i < out_mat.rows; i++)
	{
		out_mat.at<float>(i, 0) = out_pts[i].x;
		out_mat.at<float>(i, 1) = out_pts[i].y;
	}
	_out_points = out_mat;

	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_transform_point_map2image(IMGRGLOC_Image2Map* obj, CVCMat in_point, CVCMat out_point)
{
	auto p = (Image2Map*)obj;
	auto& _in_point = *(cv::Mat*)in_point;
	auto& _out_point = *(cv::Mat*)out_point;

	rloc::Point2 in_pt, out_pt;
	in_pt.x = _in_point.at<float>(0, 0);
	in_pt.y = _in_point.at<float>(0, 1);

	out_pt = p->transform_point_map2image(in_pt);

	cv::Mat pt_mat(1, 2, CV_32F);
	pt_mat.at<float>(0, 0) = out_pt.x;
	pt_mat.at<float>(0, 1) = out_pt.y;

	_out_point = pt_mat;
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_transform_points_map2image(IMGRGLOC_Image2Map* obj, CVCMat in_points, CVCMat out_points)
{
	auto p = (Image2Map*)obj;
	auto& _out_points = *(cv::Mat*)out_points;
	auto& _in_points = *(cv::Mat*)in_points;

	rloc::PointList2 in_pts;
	rloc::PointList2 out_pts;

	for (int i = 0; i < _in_points.rows; i++)
	{
		rloc::Point2 tmp;
		tmp.x = _in_points.at<float>(i, 0);
		tmp.y = _in_points.at<float>(i, 1);
		in_pts.push_back(tmp);
	}
	p->transform_point_map2image(in_pts, out_pts);
	cv::Mat out_mat(out_pts.size(), 2, CV_32F);
	for (int i = 0; i < out_mat.rows; i++)
	{
		out_mat.at<float>(i, 0) = out_pts[i].x;
		out_mat.at<float>(i, 1) = out_pts[i].y;
	}
	_out_points = out_mat;

	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_transformed_image_in_map(IMGRGLOC_Image2Map* obj, CVCMat out_img, int* out_top_left_x, int* out_top_left_y, CVCMat out_mask)
{
	auto p = (Image2Map*)obj;
	auto& _out_img = *(cv::Mat*)out_img;
	auto& _out_mask = *(cv::Mat*)out_mask;
	rloc::Point2 topleft_pt;

	p->get_transformed_image_in_map(_out_img, topleft_pt, &_out_mask);

	*out_top_left_x = topleft_pt.x;
	*out_top_left_y = topleft_pt.y;

	return IMGRGLOC_ERR_NONE;
}


IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_map_region_wrt_image(IMGRGLOC_Image2Map* obj, CVCMat out_polygon)
{
	auto p = (Image2Map*)obj;
	auto& _out_polygon = *(cv::Mat*)out_polygon;
	rloc::Polygon2 out_polygon_pts;
	p->get_map_region_wrt_image(out_polygon_pts);
	cv::Mat out_poly_mat(out_polygon_pts.size(), 2, CV_32F);
	for (int i = 0; i < out_poly_mat.rows; i++)
	{
		out_poly_mat.at<float>(i, 0) = out_polygon_pts[i].x;
		out_poly_mat.at<float>(i, 1) = out_polygon_pts[i].y;
	}
	_out_polygon = out_poly_mat;
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_map_region_wrt_map(IMGRGLOC_Image2Map* obj, CVCMat out_polygon)
{
	auto p = (Image2Map*)obj;
	auto& _out_polygon = *(cv::Mat*)out_polygon;
	rloc::Polygon2 out_polygon_pts;
	p->get_map_region_wrt_map(out_polygon_pts);
	cv::Mat out_poly_mat(out_polygon_pts.size(), 2, CV_32F);
	for (int i = 0; i < out_poly_mat.rows; i++)
	{
		out_poly_mat.at<float>(i, 0) = out_polygon_pts[i].x;
		out_poly_mat.at<float>(i, 1) = out_polygon_pts[i].y;
	}
	_out_polygon = out_poly_mat;
	return IMGRGLOC_ERR_NONE;
}


IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_matched_points_in_map(IMGRGLOC_Image2Map* obj, CVCMat out_points) {
	auto p = (Image2Map*)obj;
	rloc::PointList2 pts;
	p->get_matched_points_in_map(pts);
	cv::Mat pts_mat(pts.size(), 2, CV_32F);
	auto& _out_points = *(cv::Mat*)out_points;

	for (int i = 0; i < pts_mat.rows; i++)
	{
		pts_mat.at<float>(i, 0) = pts[i].x;
		pts_mat.at<float>(i, 1) = pts[i].y;
	}
	_out_points = pts_mat;
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_set_query_image(IMGRGLOC_Image2Map* obj, CVCMat img)
{
	auto _img = (cv::Mat*)img;
	auto p = (Image2Map*)obj;
	p->set_query_image(*_img);
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_set_map_maxlen(IMGRGLOC_Image2Map* obj, int value)
{
	auto p = (Image2Map*)obj;
	p->set_map_maxlen(value);
	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_set_query_image_maxlen(IMGRGLOC_Image2Map* obj, int value)
{
	auto p = (Image2Map*)obj;
	p->set_query_image_maxlen(value);
	return IMGRGLOC_ERR_NONE;
}

const char* IMGRGLOC_Image2Map_get_version() {
	static std::string version = "1.0";
	return version.c_str();
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_map_keypoints(IMGRGLOC_Image2Map* obj, CVCMat out_pts)
{
	auto* p = (Image2Map*)obj;
	auto& _out_pts = *(cv::Mat*)out_pts;
	auto m = p->get_processed_map();

	imgregionloc::FeaturePointListPtr pts;
	m->get_feature_point_list(pts);

	if (!pts)
		return IMGRGLOC_ERR_NONE;

	std::vector<cv::KeyPoint> _pts;
	pts->get_kps(_pts);

	_out_pts.create(_pts.size(), 2, CV_32FC1);
	for (int i = 0; i < _pts.size(); i++) {
		_out_pts.at<float>(i, 0) = _pts[i].pt.x;
		_out_pts.at<float>(i, 1) = _pts[i].pt.y;
	}

	return IMGRGLOC_ERR_NONE;
}

IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_query_image_keypoints(IMGRGLOC_Image2Map* obj, CVCMat out_pts)
{
	auto* p = (Image2Map*)obj;
	auto& _out_pts = *(cv::Mat*)out_pts;
	auto m = p->get_processed_query_image();

	imgregionloc::FeaturePointListPtr pts;
	m->get_feature_point_list(pts);

	if (!pts)
		return IMGRGLOC_ERR_NONE;

	std::vector<cv::KeyPoint> _pts;
	pts->get_kps(_pts);

	_out_pts.create(_pts.size(), 2, CV_32FC1);
	for (int i = 0; i < _pts.size(); i++) {
		_out_pts.at<float>(i, 0) = _pts[i].pt.x;
		_out_pts.at<float>(i, 1) = _pts[i].pt.y;
	}

	return IMGRGLOC_ERR_NONE;
}
