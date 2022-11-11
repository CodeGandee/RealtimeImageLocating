#pragma once

#include "imgregionloc_master.h"

namespace imgregionloc {

	class RegionTransformer
	{

	private:
		cv::Mat m_perspective_matrix;
		cv::Mat m_invert_matrix;
		int m_region_width;
		int m_region_height;
		int m_map_width;
		int m_map_height;

	public:
		void init_matrix_and_size(const cv::Mat& perspective_matrix, int region_width, int region_height, int map_width, int map_height);
		cv::Mat get_perspective_transform_matrix() const;
		void points_region_to_map(const std::vector<Point2>& src_point_list, std::vector<Point2>& dst_point_list) const;
		void points_map_to_region(const std::vector<Point2>& src_point_list, std::vector<Point2>& dst_point_list) const;
		void img_region_to_map(const cv::Mat& src, cv::Mat& dst) const;
		void img_map_to_region(const cv::Mat& src, cv::Mat& dst) const;
	};
	using RegionTransformerPtr = std::shared_ptr<RegionTransformer>;

}

