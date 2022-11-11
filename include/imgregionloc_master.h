#pragma once

#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
//#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/calib3d.hpp>
#include <mutex>
#include <vector>


namespace imgregionloc{
    using Point2 = cv::Point2f;
    using Polygon2 = std::vector<Point2>;
    using PointList2 = std::vector<Point2>;
    using ErrorCode = int;  //0=no error

    const int CVFLAG_COLOR_TO_GRAY = cv::COLOR_RGB2GRAY;

	struct ORBExtractorParam {
		int m_maxlen = -1;
		int m_nfeatures = 1000;
		float m_scaleFactor = 1.2f;
		int m_nlevels = 8;
		int m_edgeThreshold = 31;
		int m_firstLevel = 0;
		int m_WTA_K = 2;
		int m_patchSize = 31;
		int m_fastThreshold = 20;
	};
	struct SIFTExtractorParam {
		int m_maxlen = -1;
		int m_nfeatures = 0;
		int m_nOctaveLayers = 3;
		double m_contrastThreshold = 0.08;
		double m_edgeThreshold = 10;
		double m_sigma = 1.6;
		int m_maxTilt = 5;
		int m_minTile = 0;
		float m_tiltStep = 1.4142135623730951f;
		float m_rotateStepBase = 72;
	};

	struct SIFTMacherParam {
		std::string m_type = "FlannBased";
		int m_least_mpt = 10;
		int m_most_mpt = 50;
	};	
	struct ORBMacherParam {
		std::string m_type = "FlannBased";
		int m_least_mpt = 10;
		int m_most_mpt = 50;
	};
}
