#include "FeaturePointList.h"
#include <iostream>
namespace imgregionloc {

	void FeaturePointList::init_with_kp_and_desc(const std::vector<cv::KeyPoint>& input_kps, const cv::Mat& input_desc)
	{
		m_kps = input_kps;
		m_desc = input_desc;
	}

	void FeaturePointList::init_with_kp(const std::vector<cv::KeyPoint>& input_kps)
	{
		m_kps = input_kps;
	}

	void FeaturePointList::set_desc(const cv::Mat& input_desc)
	{
		m_desc = input_desc;
	}

	void FeaturePointList::get_desc(cv::Mat& desc) const
	{
		desc = m_desc;
	}

	void FeaturePointList::get_kps(std::vector<cv::KeyPoint>& kps) const
	{
		kps = m_kps;
	}

	void FeaturePointList::export_to_binary(std::vector<char>& output)
	{
	}

	void FeaturePointList::import_from_binary(const char* input, size_t n)
	{
	}

	void FeaturePointList::get_feature_point_by_idx(const int& idx, FeaturePoint& feature_point) const
	{
		feature_point.m_kp = m_kps[idx];
		feature_point.m_desc = m_desc.row(idx).clone();
	}

	void FeaturePointList::copy_to(FeaturePointList& dst) const
	{
		dst.m_kps = m_kps;
		m_desc.copyTo(dst.m_desc);
	}

}