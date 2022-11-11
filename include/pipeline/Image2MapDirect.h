#pragma once

#include "imgregionloc_master.h"
#include "Image2MapBase.h"
#include "SIFTFeatureExtractor.h"
#include "ORBFeatureExtractor.h"
#include "SIFTRegionMatcher.h"
#include "ORBRegionMatcher.h"

namespace imgregionloc{
    namespace pipeline{
        class Image2MapDirect: public Image2MapBase
        {
        public:
            Image2MapDirect() :Image2MapBase(), m_map_extractor(new SIFTFeatureExtractor), m_query_image_extractor(new SIFTFeatureExtractor), m_matcher(new SIFTRegionMatcher) {}
            
            void set_map_maxlen(int maxlen);
            void set_query_image_maxlen(int maxlen);

            void set_map(const cv::Mat& img);
            void do_map_preprocess();
            bool is_map_ready() const;  //true if we have a map and it it preprocessed

            void set_query_image(const cv::Mat& img);
            void do_query_image_keypoint_detection();
            void do_query_image_feature_extraction();
            bool is_query_image_ready() const; //true if we have a query image and it it preprocessed

            //do image to map matching
            bool do_match_image2map();  //return true if success
            //bool do_match_image2map_indirect();

            //get results
            float get_match_confidence() const;   //-1 = unsuccessful
            cv::Mat get_transform_matrix() const;
            void get_matched_points_in_map(PointList2& out) const;
            //int get_num_matched_points_in_map() const;
            void get_matched_points_in_image(PointList2& out) const;
            //int get_num_matched_points_in_image() const;

            //transformation
            Point2 transform_point_image2map(const Point2& p) const;
            void transform_point_image2map(const std::vector<Point2>& pts, std::vector<Point2>& out) const;
            Point2 transform_point_map2image(const Point2& p) const;
            void transform_point_map2image(const std::vector<Point2>& pts, std::vector<Point2>& out) const;

            //out = the transformed image
            //upper_left_point = the [out] image's upper left point in map coordinate
            //out_mask = optional output, the binary mask indicating which pixel is foreground
            void get_transformed_image_in_map(cv::Mat& out, Point2& upper_left_point, cv::Mat* out_mask=nullptr);  

            // valid map region = the region containing map pixels in image
            // valid map region in image coordinates
            void get_map_region_wrt_image(Polygon2& out) const;
            // valid map region in map coordinates
            void get_map_region_wrt_map(Polygon2& out) const;

        public:
            ImageWithFeaturePtr get_processed_map() const;
            ImageWithFeaturePtr get_processed_query_image() const;

        private:
            SIFTFeatureExtractorPtr m_map_extractor;
            SIFTFeatureExtractorPtr m_query_image_extractor;
            SIFTRegionMatcherPtr m_matcher;
            bool m_map_ready = false;
            bool m_query_image_ready = false;
        };
        using Image2MapDirectPtr = std::shared_ptr<Image2MapDirect>;

    };
}    