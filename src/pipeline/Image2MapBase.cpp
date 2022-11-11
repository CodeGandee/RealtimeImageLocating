#include "pipeline/Image2MapBase.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

using namespace std;
using namespace cv;
namespace bg = boost::geometry;

namespace imgregionloc{
namespace pipeline{
	void Image2MapBase::set_map(const ImageWithFeaturePtr& fullmap)
	{
		m_map = fullmap;
	}
	void Image2MapBase::set_map(const cv::Mat& img)
	{
		m_map->set_image(img);
		m_map_width = img.cols;
		m_map_height = img.rows;
		
	}

	void Image2MapBase::set_query_image(const ImageWithFeaturePtr& query_image)
	{
		m_query_image = query_image;
	}

	void Image2MapBase::set_query_image(const cv::Mat& img)
	{
		m_query_image->set_image(img);
		m_image_width = img.cols;
		m_image_height = img.rows;
	}

	bool Image2MapBase::do_match_image2map()
	{	
		return false;
		
	}

	cv::Mat Image2MapBase::get_transform_matrix() const
	{
		return Mat();
	}

	Point2 Image2MapBase::transform_point_image2map(const Point2& p) const
	{
		vector<Point2> src(1);
		vector<Point2> out(1);
		src[0] = p;
		m_transformer->points_region_to_map(src, out);
		return out[0];
	}
	void Image2MapBase::transform_point_image2map(const std::vector<Point2>& pts, std::vector<Point2>& out) const
	{
		m_transformer->points_region_to_map(pts, out);
	}
	Point2 Image2MapBase::transform_point_map2image(const Point2& p) const
	{
		vector<Point2> src(1);
		vector<Point2> out(1);
		src[0] = p;
		m_transformer->points_map_to_region(src, out);
		return out[0];
	}
	void Image2MapBase::transform_point_map2image(const std::vector<Point2>& pts, std::vector<Point2>& out) const
	{
		m_transformer->points_map_to_region(pts, out);
	}
	void Image2MapBase::get_transformed_image_in_map(cv::Mat& out, Point2& upper_left_point, cv::Mat* out_mask)
	{
		Mat img_ori, img_new, img_gray;
		
		m_query_image->get_image(img_ori);
		m_transformer->img_region_to_map(img_ori, img_new);
		Polygon2 map_region_polygon;
		float top_left_x(m_map_width), top_left_y(m_map_height), bottom_right_x(0), bottom_right_y(0);
		get_map_region_wrt_map(map_region_polygon);
		for (int i = 0; i < map_region_polygon.size(); i++)
		{
			top_left_x = min(map_region_polygon[i].x, top_left_x);
			top_left_y = min(map_region_polygon[i].y, top_left_y);
			bottom_right_x = max(map_region_polygon[i].x, bottom_right_x);
			bottom_right_y = max(map_region_polygon[i].y, bottom_right_y);
		}
		Rect bbox(top_left_x, top_left_y, max(1.0f,bottom_right_x-top_left_x), max(1.0f,bottom_right_y-top_left_y));
		upper_left_point.x = bbox.x;
		upper_left_point.y = bbox.y;
		img_new(bbox).copyTo(out);
		
		if (!out_mask)
			return;
		out_mask->create(out.rows, out.cols, CV_8UC1);
		out_mask->setTo(0);
		Polygon2 corners(4);
		corners[0] = Point2(0, 0);
		corners[1] = Point2(m_image_width, 0);
		corners[2] = Point2(m_image_width, m_image_height);
		corners[3] = Point2(0, m_image_height);
		transform_point_image2map(corners, corners);
		std::vector<cv::Point> fillContSingle;
		Point2 anchor(corners[0].x+ upper_left_point.x, corners[0].y+ upper_left_point.y);
		for (size_t i = 0; i < corners.size(); i++)
		{
			fillContSingle.push_back(corners[i]);
		}
		cv::fillPoly(*out_mask, std::vector<std::vector<cv::Point>>{fillContSingle}, cv::Scalar(255));

	}
	void Image2MapBase::get_map_region_wrt_image(Polygon2& out) const
	{
		
		//Polygon2 polygon_out;
		Polygon2 map_region;
		get_map_region_wrt_map(map_region);
		transform_point_map2image(map_region, out);

	}	
	void Image2MapBase::get_map_region_wrt_map(Polygon2& out) const
	{
		Polygon2 image_corners(5), polygon_out;
		image_corners[0] = Point2(0, 0);
		image_corners[1] = Point2(0, m_image_height);
		image_corners[2] = Point2(m_image_width, m_image_height);
		image_corners[3] = Point2(m_image_width, 0);
		image_corners[4] = Point2(0, 0);
		transform_point_image2map(image_corners, image_corners);

		typedef bg::model::d2::point_xy<float> bg_point;
		typedef bg::model::polygon<bg_point> bg_polygon;
		std::vector<bg_polygon> output;
		bg_polygon map_corners, transformed_image_corners;
		bg::append(map_corners.outer(), bg_point(0, 0));
		bg::append(map_corners.outer(), bg_point(0, m_map_height));
		bg::append(map_corners.outer(), bg_point(m_map_width, m_map_height));
		bg::append(map_corners.outer(), bg_point(m_map_width, 0));
		bg::append(map_corners.outer(), bg_point(0, 0));

		for (int i = 0; i < image_corners.size(); i++)
		{
			bg::append(transformed_image_corners.outer(), bg_point(image_corners[i].x, image_corners[i].y));
		}
		bg::correct(map_corners);
		bg::correct(transformed_image_corners);
		bg::intersection(map_corners, transformed_image_corners, output);
		if (output.empty())
		{
			polygon_out = image_corners;
		}
		else
		{
			for (int i = 0; i < output[0].outer().size() - 1; i++)
			{
				polygon_out.push_back(Point2(output[0].outer().data()[i].x(), output[0].outer().data()[i].y()));
			}
		}

		out = polygon_out;
	}
}
}