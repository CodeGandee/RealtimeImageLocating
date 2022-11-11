#include "RegionTransformer.h"

using namespace cv;
using namespace std;

namespace imgregionloc {
	void RegionTransformer::init_matrix_and_size(const cv::Mat& perspective_matrix, int region_width, int region_height, int map_width, int map_height)
	{
		perspective_matrix.copyTo(m_perspective_matrix);
		invert(m_perspective_matrix, m_invert_matrix);
		m_region_width = region_width;
		m_region_height = region_height;
		m_map_width = map_width;
		m_map_height = map_height;
	}
	cv::Mat RegionTransformer::get_perspective_transform_matrix() const
	{
		return m_perspective_matrix;
	}
	void RegionTransformer::points_region_to_map(const std::vector<Point2>& src_point_list, std::vector<Point2>& dst_point_list) const
	{
		perspectiveTransform(src_point_list, dst_point_list, m_perspective_matrix);
		
	}
	void RegionTransformer::points_map_to_region(const std::vector<Point2>& src_point_list, std::vector<Point2>& dst_point_list) const
	{
		perspectiveTransform(src_point_list, dst_point_list, m_invert_matrix);
	}

	void RegionTransformer::img_region_to_map(const cv::Mat& src, cv::Mat& dst) const
	{
		warpPerspective(src, dst, m_perspective_matrix, Size(m_map_width, m_map_height));
	}

	void RegionTransformer::img_map_to_region(const cv::Mat& src, cv::Mat& dst) const
	{
		warpPerspective(src, dst, m_perspective_matrix, Size(m_region_width, m_region_height), WARP_INVERSE_MAP);
	}

}

