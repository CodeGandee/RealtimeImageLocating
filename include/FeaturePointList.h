#pragma once

#include "imgregionloc_master.h"

namespace imgregionloc {

	struct FeaturePoint
	{
		cv::KeyPoint m_kp;
		cv::Mat m_desc;
		void copy_to(FeaturePoint& p) const {
			p = *this;
			p.m_desc = m_desc.clone();
		}
	};

	class FeaturePointList
	{
	private:
		std::vector<cv::KeyPoint> m_kps;
		cv::Mat m_desc;

	public:
		void init_with_kp_and_desc(const std::vector<cv::KeyPoint>& input_kps, const cv::Mat& input_desc);
		void init_with_kp(const std::vector<cv::KeyPoint>& input_kps);
		void set_desc(const cv::Mat& input_desc);
		void get_desc(cv::Mat& desc) const;
		void get_kps(std::vector<cv::KeyPoint>& kps) const;
		void export_to_binary(std::vector<char>& output);
		void import_from_binary(const char* input, size_t n);
		void get_feature_point_by_idx(const int& idx, FeaturePoint& feature_point) const;
		void copy_to(FeaturePointList& dst) const;

	};
	using FeaturePointListPtr = std::shared_ptr<FeaturePointList>;
}


