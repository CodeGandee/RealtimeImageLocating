#pragma once

#include "imgregionloc_master.h"
#include "ImageWithFeature.h"

namespace imgregionloc {
	class ORBRegionMatcher
	{
		
	private:
		std::vector<Point2> m_query_pt;
		std::vector<Point2> m_map_pt;
		int m_upper_limit;
		int m_lower_limit;
		cv::Ptr<cv::DescriptorMatcher> matcher;
		ImageWithFeaturePtr m_map;
		ImageWithFeaturePtr m_query;
		cv::Mat m_perspective_transform_matrix;
		ORBMacherParam m_init_param;

	public:
		ORBRegionMatcher() : m_map(new ImageWithFeature), m_query(new ImageWithFeature) {
			init_with_params(m_init_param);
		}
		void init_with_params(const ORBMacherParam& p);
		void set_map_feature(ImageWithFeaturePtr map_features);
		void set_query_image_feature(ImageWithFeaturePtr query_features);
		void process_match();
		void get_matched_points_in_map(PointList2& out);
		void get_matched_points_in_image(PointList2& out);
		void get_perspective_transform_matrix(cv::Mat& perspective_transform_matrix) const;
		void check_match_success(bool& success) const;
		void get_confidence(float& confidence) const;

	};
	using ORBRegionMatcherPtr = std::shared_ptr<ORBRegionMatcher>;
}



