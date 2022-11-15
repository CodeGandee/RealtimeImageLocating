#pragma once

#include "imgregionloc_master.h"
#include "FeaturePointList.h"

namespace imgregionloc {
	class ImageWithFeature
	{
	private:

		cv::Mat m_image;	//original image
		FeaturePointListPtr m_feature_point_list;
		bool m_image_ready = false;
		bool m_feature_ready = false;
		bool m_matched = false;
		float m_scale_factor;

	public:
		ImageWithFeature() :m_feature_point_list(new FeaturePointList) {}
		void set_image(const cv::Mat& img);
		void set_feature_point_list(const std::vector<cv::KeyPoint>& input_kps, const cv::Mat& input_desc);
		void set_keypoint_list(const std::vector<cv::KeyPoint>& input_kps);
		void set_feature(const cv::Mat& input_desc);
		void get_image(cv::Mat& image) const;
		void get_feature_point_list(FeaturePointListPtr& feature_point_list) const;
		FeaturePointListPtr get_feature_point_list() const;
		void set_matched(bool flag);
		bool check_matched();
		bool check_ready();
		float get_scale_factor();
		void set_scale_factor(float factor);

		void copy_to(ImageWithFeature& dst) const;

	};
	

	using ImageWithFeaturePtr = std::shared_ptr<ImageWithFeature>;
	//using ImageWithFeatureConstPtr = std::shared_ptr<const ImageWithFeature>;
}


