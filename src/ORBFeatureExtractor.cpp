#include "ORBFeatureExtractor.h"

using namespace std;
using namespace cv;
namespace imgregionloc {

	void ORBFeatureExtractor::set_image(const Mat& img)
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
			m_scale_factor =  m_maxlen/internal_image_maxlen;
		}
		
		m_image_with_feature->set_scale_factor(m_scale_factor);
		resize(m_image_internal, m_image_internal, Size(), m_scale_factor, m_scale_factor);
	}

	void ORBFeatureExtractor::init_with_params(const ORBExtractorParam& p)
	{
		m_maxlen = p.m_maxlen;
		m_extractor = ORB::create(p.m_nfeatures, p.m_scaleFactor, p.m_nlevels, p.m_edgeThreshold, p.m_firstLevel, p.m_WTA_K, ORB::HARRIS_SCORE, p.m_patchSize, p.m_fastThreshold);
	}

	void ORBFeatureExtractor::process_keypoint()
	{
		vector<KeyPoint> kps;
		m_extractor->detect(m_image_internal, kps);
		m_image_with_feature->set_keypoint_list(kps);
	}

	void ORBFeatureExtractor::process_feature()
	{
		vector<KeyPoint> kps;
		Mat desc;
		FeaturePointListPtr fp;

		m_image_with_feature->get_feature_point_list(fp);
		fp->get_kps(kps);
		m_extractor->compute(m_image_internal, kps, desc);
		m_image_with_feature->set_feature(desc);
	}



	void ORBFeatureExtractor::process_keypoint_and_feature()
	{
		vector<KeyPoint> kps;
		Mat desc;
		m_extractor->detectAndCompute(m_image_internal, Mat(), kps, desc);
		m_image_with_feature->set_feature_point_list(kps, desc);
	}

	void ORBFeatureExtractor::set_keypoints(const std::vector<KeyPoint>& input_kps, const Mat& input_desc)
	{
		m_image_with_feature->set_feature_point_list(input_kps, input_desc);
	}

	void ORBFeatureExtractor::get_image_with_feature(ImageWithFeaturePtr& image_with_feature) const
	{
		image_with_feature = m_image_with_feature;
	}

}