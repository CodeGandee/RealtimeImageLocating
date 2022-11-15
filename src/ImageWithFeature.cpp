#include "ImageWithFeature.h"
#include <iostream>
using namespace std;
using namespace cv;

namespace imgregionloc {

	void ImageWithFeature::set_image(const cv::Mat& img)
	{
		m_image = img;
		m_image_ready = true;
		m_feature_ready = false;
	}

	void ImageWithFeature::set_feature_point_list(const std::vector<cv::KeyPoint>& input_kps, const cv::Mat& input_desc)
	{
		m_feature_point_list->init_with_kp_and_desc(input_kps, input_desc);
		m_feature_ready = true;
	}

	void ImageWithFeature::set_keypoint_list(const std::vector<cv::KeyPoint>& input_kps)
	{
		m_feature_point_list->init_with_kp(input_kps);
	}

	void ImageWithFeature::set_feature(const cv::Mat& input_desc)
	{
		m_feature_point_list->set_desc(input_desc);
		m_feature_ready = true;
	}

	void ImageWithFeature::get_image(Mat& image) const
	{
		if (m_image_ready)
		{
			m_image.copyTo(image);
		}
	}
	void ImageWithFeature::get_feature_point_list(FeaturePointListPtr& feature_point_list) const
	{
		feature_point_list = m_feature_point_list;

	}
	FeaturePointListPtr ImageWithFeature::get_feature_point_list() const
	{
		return m_feature_point_list;
	}
	void ImageWithFeature::set_matched(bool flag)
	{
		m_matched = flag;
	}
	bool ImageWithFeature::check_matched()
	{
		return m_matched;
	}
	bool ImageWithFeature::check_ready()
	{
		return (m_image_ready && m_feature_ready);
	}
	float ImageWithFeature::get_scale_factor()
	{
		return m_scale_factor;
	}
	void ImageWithFeature::set_scale_factor(float factor)
	{
		m_scale_factor = factor;
	}
	void ImageWithFeature::copy_to(ImageWithFeature& dst) const
	{
		m_image.copyTo(dst.m_image);
		m_feature_point_list->copy_to(*dst.m_feature_point_list);
		dst.m_image_ready = m_image_ready;
		dst.m_feature_ready = m_feature_ready;
		dst.m_matched = m_matched;
		dst.m_scale_factor=m_scale_factor;
	}
}