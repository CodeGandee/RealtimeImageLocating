#include "ORBRegionMatcher.h"

using namespace std;
using namespace cv;

namespace imgregionloc {
    void ORBRegionMatcher::init_with_params(const ORBMacherParam& p)
    {
        matcher = makePtr<FlannBasedMatcher>(makePtr<flann::LshIndexParams>(6, 12, 1));
        m_upper_limit = p.m_most_mpt;
        m_lower_limit = p.m_least_mpt;
    }

    void ORBRegionMatcher::set_map_feature(ImageWithFeaturePtr map_features)
    {
        m_map = map_features;
        FeaturePointListPtr map_feature_point_list;
        Mat map_desc;
        m_map->get_feature_point_list(map_feature_point_list);
        map_feature_point_list->get_desc(map_desc);
        matcher->clear();
        matcher->add(map_desc);
        matcher->train();
    }

    void ORBRegionMatcher::set_query_image_feature(ImageWithFeaturePtr query_features)
    {
        m_query = query_features;
    }

    void ORBRegionMatcher::process_match()
    {
        vector< vector<DMatch> > rawMatches;
        vector<Point2> p1, p2;
        vector<KeyPoint> map_kps, query_kps;
        Mat map_desc, query_desc;
        FeaturePointListPtr map_feature_point_list;
        FeaturePointListPtr query_feature_point_list;
        m_query->get_feature_point_list(query_feature_point_list);
        m_map->get_feature_point_list(map_feature_point_list);
        query_feature_point_list->get_desc(query_desc);
        query_feature_point_list->get_kps(query_kps);
        map_feature_point_list->get_desc(map_desc);
        map_feature_point_list->get_kps(map_kps);

        matcher->knnMatch(query_desc, rawMatches, 2);
        for (size_t i = 0; i < rawMatches.size(); i++)
        {
            const vector<DMatch>& m = rawMatches[i];
            if (m.size() == 2 && m[0].distance < m[1].distance * 0.75)
            {
                p1.push_back(query_kps[m[0].queryIdx].pt);
                p2.push_back(map_kps[m[0].trainIdx].pt);
            }
        }
        m_query_pt = vector<Point2>(p1.size());
        m_map_pt = vector<Point2>(p2.size());
        for (int i = 0; i < m_query_pt.size(); i++)
        {
            m_query_pt[i] = p1[i] / m_query->get_scale_factor();
            m_map_pt[i] = p2[i] / m_map->get_scale_factor();
        }
        vector<uchar> status;

        if (m_query_pt.size() >= 4)
        {
			try
			{
				m_perspective_transform_matrix = findHomography(m_query_pt, m_map_pt, status, RANSAC);
			}
			catch (const cv::Exception&)
			{
				m_perspective_transform_matrix = Mat();
			}
        }
        else
        {
            m_perspective_transform_matrix = Mat();
        }
    }
    void ORBRegionMatcher::get_matched_points_in_map(PointList2& out)
    {
        out = m_map_pt;
    }
    void ORBRegionMatcher::get_matched_points_in_image(PointList2& out)
    {
        out = m_query_pt;
    }
    void ORBRegionMatcher::get_perspective_transform_matrix(cv::Mat& perspective_transform_matrix) const
    {
        perspective_transform_matrix = m_perspective_transform_matrix.clone();
    }
    void ORBRegionMatcher::check_match_success(bool& success) const
    {
        if (m_perspective_transform_matrix.dims == 2 && m_perspective_transform_matrix.cols == 3)
        {
            success = true;
        }
        else
        {
            success = false;
        }
    }
    void ORBRegionMatcher::get_confidence(float& confidence) const
    {
        float raw_c = 0.0f;
        if (m_query_pt.size() != m_map_pt.size() || m_query_pt.size() < m_lower_limit)
        {
            raw_c = 0.0f;
        }
        else
        {
            raw_c = float(m_query_pt.size()) / float(m_upper_limit);
        }
        confidence = std::min(1.0f, raw_c);
    }

}