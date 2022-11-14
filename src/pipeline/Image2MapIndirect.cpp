#include "pipeline/Image2MapIndirect.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

using namespace std;
using namespace cv;
namespace bg = boost::geometry;

namespace imgregionloc {
	namespace pipeline {
		void Image2MapIndirect::set_map_maxlen(int maxlen)
		{
			SIFTExtractorParam sift_p;
			sift_p.m_maxlen = maxlen;
			m_map_sift_extractor->init_with_params(sift_p);
		}
		void Image2MapIndirect::set_ref_image_maxlen(int maxlen)
		{
			SIFTExtractorParam sift_p;
			sift_p.m_maxlen = maxlen;
			m_ref_image_sift_extractor->init_with_params(sift_p);
		}
		void Image2MapIndirect::set_query_image_maxlen(int maxlen)
		{
			
			ORBExtractorParam orb_p;
			orb_p.m_maxlen = maxlen;
			m_ref_image_orb_extractor->init_with_params(orb_p);
			m_query_image_orb_extractor->init_with_params(orb_p);
		}
		void Image2MapIndirect::set_map(const cv::Mat& img)
		{
			m_map->set_image(img);
			m_map_sift_extractor->set_image(img);
			m_map_width = img.cols;
			m_map_height = img.rows;
			m_map_ready = false;

		}
		void Image2MapIndirect::do_map_preprocess()
		{
			m_map_sift_extractor->process_keypoint_and_feature();
			m_map_sift_extractor->get_image_with_feature(m_map);
			std::lock_guard<std::mutex> lock(m_sift_matcher_lock);
			m_sift_matcher->set_map_feature(m_map);
			m_map_ready = true;
		}
		bool Image2MapIndirect::is_map_ready() const
		{
			return m_map_ready;
		}
		void Image2MapIndirect::set_ref_image(const cv::Mat& img)
		{
			std::lock_guard<std::mutex> lock(m_lock_ref_image_to_match);
			m_ref_image_to_match.reset();
			m_ref_image_to_match.m_image = img;
			m_ref_image2map_ready = false;
		}

		void Image2MapIndirect::do_ref_image_keypoint_detection()
		{
			
			auto& img = m_ref_image_to_match;
			auto& sift = m_ref_image_sift_extractor;
			auto& orb = m_ref_image_orb_extractor;
			{
				std::lock_guard<std::mutex> lock(m_lock_ref_image_to_match);
				sift->set_image(img.m_image);
				orb->set_image(img.m_image);
			}
			sift->process_keypoint();
			orb->process_keypoint();
			{
				std::lock_guard<std::mutex> lock(m_lock_ref_image_to_match);
				sift->get_image_with_feature(img.m_sift);
				orb->get_image_with_feature(img.m_orb);
			}

		}

		void Image2MapIndirect::do_ref_image_feature_extraction()
		{
			auto& img = m_ref_image_to_match;
			auto& sift = m_ref_image_sift_extractor;
			auto& orb = m_ref_image_orb_extractor;

			sift->process_feature();
			orb->process_feature();
			std::lock_guard<std::mutex> lock(m_lock_ref_image_to_match);
			sift->get_image_with_feature(img.m_sift);
			orb->get_image_with_feature(img.m_orb);
			img.m_is_ready = true;

		}
		bool Image2MapIndirect::is_ref_image2map_ready() const
		{
			return m_ref_image_to_match.m_is_ready;
		}
		bool Image2MapIndirect::is_ref_image2query_ready() const
		{
			return m_ref_image.m_is_ready;
		}
		void Image2MapIndirect::set_query_image(const cv::Mat& img)
		{
			std::lock_guard<std::mutex> lock(m_lock_query_image);
			m_query_image.reset();
			m_query_image.m_image = img;
			m_image_width = img.cols;
			m_image_height = img.rows;
		}
		void Image2MapIndirect::do_query_image_keypoint_detection()
		{
			std::lock_guard<std::mutex> lock(m_lock_query_image);
			auto& img = m_query_image;
			auto& orb = m_query_image_orb_extractor;
			orb->set_image(img.m_image);
			orb->process_keypoint();
			orb->get_image_with_feature(img.m_orb);
		}
		void Image2MapIndirect::do_query_image_feature_extraction()
		{
			std::lock_guard<std::mutex> lock(m_lock_query_image);
			auto& img = m_query_image;
			auto& orb = m_query_image_orb_extractor;
			orb->process_feature();
			orb->get_image_with_feature(img.m_orb);
			img.m_is_ready = true;
		}

		bool Image2MapIndirect::is_query_image_ready() const
		{
			return m_query_image.m_is_ready;
		}
		void Image2MapIndirect::clear_query_image()
		{
			m_query_image.reset();
		}

		bool Image2MapIndirect::do_match_image2map()
		{
			std::lock_guard<std::mutex> lock(m_lock_direct_match);
			bool ref_ready;
			{
				std::lock(m_lock_ref_image_to_match, m_sift_matcher_lock);
				m_sift_matcher->set_query_image_feature(m_ref_image_to_match.m_sift);
				ref_ready = m_ref_image_to_match.m_is_ready;
				m_lock_ref_image_to_match.unlock();
				m_sift_matcher_lock.unlock();
			}
			if (ref_ready && m_map_ready) {
				cv::Mat perspective_transform_matrix;
				{
					std::lock_guard<std::mutex> lock(m_sift_matcher_lock);
					m_sift_matcher->process_match();
					m_sift_matcher->get_perspective_transform_matrix(perspective_transform_matrix);
				}
				if (perspective_transform_matrix.empty())
				{
					return false;
				}
				std::lock(m_lock_ref_image, m_lock_ref_image_to_match, m_orb_matcher_lock, m_sift_matcher_lock);
				perspective_transform_matrix.copyTo(m_ref_image_to_match.m_transform);
				m_sift_matcher->get_confidence(m_ref_image_to_match.m_match_confidence);
				m_ref_image.reset();
				m_ref_image_to_match.copy_to(m_ref_image);
				m_orb_matcher->set_map_feature(m_ref_image.m_orb);
				m_orb_matcher_lock.unlock();
				m_lock_ref_image.unlock();
				m_lock_ref_image_to_match.unlock();
				m_sift_matcher_lock.unlock();
				if (m_query_image.m_transform.empty())
				{
					std::lock_guard<std::mutex> lock(m_transformer_lock);
					m_transformer->init_matrix_and_size(m_ref_image.m_transform, m_image_width, m_image_height, m_map_width, m_map_height);
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		bool Image2MapIndirect::do_match_image2map_indirect()
		{
			std::lock_guard<std::mutex> lock(m_lock_indirect_match);
			bool ref_ready, query_ready;
			{
				std::lock(m_lock_ref_image, m_orb_matcher_lock);
				m_orb_matcher->set_query_image_feature(m_query_image.m_orb);
				ref_ready = m_ref_image.m_is_ready;
				query_ready = m_query_image.m_is_ready;
				m_orb_matcher_lock.unlock();
				m_lock_ref_image.unlock();
			}
			if (ref_ready && query_ready) {
				{
					std::lock_guard<std::mutex> lock(m_orb_matcher_lock);
					m_orb_matcher->process_match();
				}
				cv::Mat perspective_transform_matrix;
				std::lock(m_lock_ref_image, m_lock_query_image, m_orb_matcher_lock, m_transformer_lock);
				m_orb_matcher->get_perspective_transform_matrix(perspective_transform_matrix);
				if (perspective_transform_matrix.empty() || m_ref_image.m_transform.empty())
				{
					return false;
				}
				perspective_transform_matrix.copyTo(m_query_image.m_transform);
				m_orb_matcher->get_confidence(m_query_image.m_match_confidence);
				m_transformer->init_matrix_and_size(m_query_image.m_transform * m_ref_image.m_transform, m_image_width, m_image_height, m_map_width, m_map_height);
				m_lock_ref_image.unlock();
				m_lock_query_image.unlock();
				m_orb_matcher_lock.unlock();
				m_transformer_lock.unlock();
				return true;
			}
			else
			{
				return false;
			}
		}
		float Image2MapIndirect::get_match_confidence() const
		{
			return m_query_image.m_match_confidence * m_ref_image.m_match_confidence;
		}
		cv::Mat Image2MapIndirect::get_transform_matrix() const
		{
			return m_transformer->get_perspective_transform_matrix();
		}

		void Image2MapIndirect::get_matched_points_in_map(PointList2& out) const
		{
		}

		void Image2MapIndirect::get_matched_points_in_image(PointList2& out) const
		{
		}

		Point2 Image2MapIndirect::transform_point_image2map(const Point2& p) const
		{
			vector<Point2> src(1);
			vector<Point2> out(1);
			src[0] = p;
			m_transformer->points_region_to_map(src, out);
			return out[0];
		}
		void Image2MapIndirect::transform_point_image2map(const std::vector<Point2>& pts, std::vector<Point2>& out) const
		{
			m_transformer->points_region_to_map(pts, out);
		}
		Point2 Image2MapIndirect::transform_point_map2image(const Point2& p) const
		{
			vector<Point2> src(1);
			vector<Point2> out(1);
			src[0] = p;
			m_transformer->points_map_to_region(src, out);
			return out[0];
		}
		void Image2MapIndirect::transform_point_map2image(const std::vector<Point2>& pts, std::vector<Point2>& out) const
		{
			m_transformer->points_map_to_region(pts, out);
		}
		void Image2MapIndirect::get_transformed_image_in_map(cv::Mat& out, Point2& upper_left_point, cv::Mat* out_mask)
		{
			Mat img_ori, img_new, img_gray;
			{
				std::lock_guard<std::mutex> lock(m_lock_query_image);
				m_query_image.m_orb->get_image(img_ori);
			}
			std::lock_guard<std::mutex> lock(m_transformer_lock);
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
			Rect bbox(top_left_x, top_left_y, max(1.0f, bottom_right_x - top_left_x), max(1.0f, bottom_right_y - top_left_y));
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
			Point2 anchor(corners[0].x + upper_left_point.x, corners[0].y + upper_left_point.y);
			for (size_t i = 0; i < corners.size(); i++)
			{
				fillContSingle.push_back(corners[i]);
			}
			cv::fillPoly(*out_mask, std::vector<std::vector<cv::Point>>{fillContSingle}, cv::Scalar(255));

		}
		void Image2MapIndirect::get_map_region_wrt_image(Polygon2& out) const
		{
			Polygon2 map_region;
			get_map_region_wrt_map(map_region);
			transform_point_map2image(map_region, out);

		}
		void Image2MapIndirect::get_map_region_wrt_map(Polygon2& out) const
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

		ImageWithFeaturePtr Image2MapIndirect::get_processed_map() const
		{
			return m_map;
		}

		ImageWithFeaturePtr Image2MapIndirect::get_processed_orb_ref() const
		{
			return m_ref_image.m_orb;
		}

		ImageWithFeaturePtr Image2MapIndirect::get_processed_sift_ref() const
		{
			return m_ref_image.m_sift;
		}

		ImageWithFeaturePtr Image2MapIndirect::get_processed_query_image() const
		{
			return m_query_image.m_orb;
		}

		cv::Mat Image2MapIndirect::get_map_img()
		{
			cv::Mat output;
			m_map->get_image(output);
			return output;
		}

		cv::Mat Image2MapIndirect::get_query2ref_img()
		{
			cv::Mat output;
			m_ref_image.m_orb->get_image(output);
			return output;
		}

		cv::Mat Image2MapIndirect::get_ref2map_img()
		{
			cv::Mat output;
			m_ref_image.m_sift->get_image(output);
			return output;
		}

		cv::Mat Image2MapIndirect::get_query_img()
		{
			cv::Mat output;
			std::lock_guard<std::mutex> lock(m_lock_query_image);
			m_query_image.m_orb->get_image(output);
			return output;
		}

	}
}