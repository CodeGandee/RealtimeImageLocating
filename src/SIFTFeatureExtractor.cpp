#include "SIFTFeatureExtractor.h"

using namespace std;
using namespace cv;
namespace imgregionloc {

	void SIFTFeatureExtractor::set_image(const Mat& img)
	{
		m_image = img;
		m_image_with_feature->set_image(img);

		if (img.channels() == 1) //gray image
			m_image_internal = img.clone();
		else
			cv::cvtColor(img, m_image_internal, CVFLAG_COLOR_TO_GRAY);
		float internal_image_maxlen = max(m_image_internal.cols, m_image_internal.rows);
		if (m_maxlen<1 || m_maxlen>internal_image_maxlen)
		{
			m_scale_factor = 1.0f;
		}
		else
		{
			m_scale_factor = m_maxlen / internal_image_maxlen;
		}

		m_image_with_feature->set_scale_factor(m_scale_factor);
		resize(m_image_internal, m_image_internal, Size(), m_scale_factor, m_scale_factor);
	}

	void SIFTFeatureExtractor::init_with_params(const SIFTExtractorParam& p)
	{
		m_maxlen = p.m_maxlen;
		m_extractor = SIFT::create(p.m_nfeatures, p.m_nOctaveLayers, p.m_contrastThreshold, p.m_edgeThreshold, p.m_sigma);
		//m_extractor = SIFT::create();
	}

	void SIFTFeatureExtractor::process_keypoint()
	{
		vector<KeyPoint> kps;
		m_extractor->detect(m_image_internal, kps);
		m_image_with_feature->set_keypoint_list(kps);
	}

	void SIFTFeatureExtractor::process_feature()
	{
		vector<KeyPoint> kps;
		Mat desc;
		FeaturePointListPtr fp;
		
		m_image_with_feature->get_feature_point_list(fp);
		fp->get_kps(kps);
		m_extractor->compute(m_image_internal, kps, desc);
		m_image_with_feature->set_feature(desc);
	}



	void SIFTFeatureExtractor::process_keypoint_and_feature()
	{
		vector<KeyPoint> kps;
		Mat desc;
		m_extractor->detectAndCompute(m_image_internal, Mat(), kps, desc);
		m_image_with_feature->set_feature_point_list(kps, desc);
	}

	void SIFTFeatureExtractor::set_keypoints(const std::vector<KeyPoint>& input_kps, const Mat& input_desc)
	{
		m_image_with_feature->set_feature_point_list(input_kps, input_desc);
	}

	void SIFTFeatureExtractor::get_image_with_feature(ImageWithFeaturePtr& image_with_feature) const
	{
		image_with_feature = m_image_with_feature;
	}
	ImageWithFeaturePtr SIFTFeatureExtractor::get_image_with_feature() const
	{
		return m_image_with_feature;
	}
}