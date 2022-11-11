#pragma once

#include "imgregionloc_master.h"
#include "Image2MapBase.h"
#include "SIFTFeatureExtractor.h"
#include "ORBFeatureExtractor.h"
#include "SIFTRegionMatcher.h"
#include "ORBRegionMatcher.h"

namespace imgregionloc{
namespace pipeline{

    class Image2MapIndirect : public Image2MapBase
    {
    private:
        //multi-feature image with transformation to a target
        struct ImageMatchData
        {
            //cv::Mat m_ref_img;
            //cv::Mat m_ref2map_perpective_matrix;

            //the original image
            cv::Mat m_image;

            //same image, different feature
            ImageWithFeaturePtr m_sift;
            ImageWithFeaturePtr m_orb;

            //3x3 transform from m_sift/m_orb to target image
            cv::Mat m_transform;
            float m_match_confidence = -1;
            bool m_is_ready = false;

            bool matched() const {
                return m_match_confidence > 0;
            }

            ImageMatchData() {
                reset();
            }            

            void copy_to(ImageMatchData& dst) const {
                m_image.copyTo(dst.m_image);
                m_sift->copy_to(*dst.m_sift);
                m_orb->copy_to(*dst.m_orb);
                m_transform.copyTo(dst.m_transform);
                dst.m_match_confidence = m_match_confidence;
                dst.m_is_ready = m_is_ready;
            }

            void reset() {
                m_image = cv::Mat();
                m_sift.reset(new ImageWithFeature());
                m_orb.reset(new ImageWithFeature());
                m_match_confidence = -1;
                m_transform = cv::Mat();
                m_is_ready = false;
                m_transform = cv::Mat::eye({ 3,3 }, CV_32FC1);
            }
        };
    public:
        Image2MapIndirect() :Image2MapBase(), m_map_sift_extractor(new SIFTFeatureExtractor), m_ref_image_sift_extractor(new SIFTFeatureExtractor),
            m_sift_matcher(new SIFTRegionMatcher), m_ref_image_orb_extractor(new ORBFeatureExtractor), m_query_image_orb_extractor(new ORBFeatureExtractor),
            m_orb_matcher(new ORBRegionMatcher) {}

        void set_map_maxlen(int maxlen);
        void set_ref_image_maxlen(int maxlen);
        void set_query_image_maxlen(int maxlen);

        void set_map(const cv::Mat& img);
        void do_map_preprocess();
        bool is_map_ready() const;  //true if we have a map and it it preprocessed

        void set_ref_image(const cv::Mat& img);
        void do_ref_image_keypoint_detection();
        void do_ref_image_feature_extraction();
        bool is_ref_image2query_ready() const;
        bool is_ref_image2map_ready() const; 

        void set_query_image(const cv::Mat& img);
        void do_query_image_keypoint_detection();
        void do_query_image_feature_extraction();
        bool is_query_image_ready() const; //true if we have a query image and it it preprocessed

        bool do_match_image2map();  //return true if success
        bool do_match_image2map_indirect();

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
        void get_transformed_image_in_map(cv::Mat& out, Point2& upper_left_point, cv::Mat* out_mask = nullptr);

        // valid map region = the region containing map pixels in image
        // valid map region in image coordinates
        void get_map_region_wrt_image(Polygon2& out) const;
        // valid map region in map coordinates
        void get_map_region_wrt_map(Polygon2& out) const;

    public:
        ImageWithFeaturePtr get_processed_map() const;
        ImageWithFeaturePtr get_processed_orb_ref() const;
        ImageWithFeaturePtr get_processed_sift_ref() const;
        ImageWithFeaturePtr get_processed_query_image() const;

    public:
        //for debuging
        cv::Mat get_map_img();
        cv::Mat get_query2ref_img();
        cv::Mat get_ref2map_img();
        cv::Mat get_query_img();

    private:

        SIFTFeatureExtractorPtr m_map_sift_extractor;
        ORBFeatureExtractorPtr m_ref_image_orb_extractor;
        SIFTFeatureExtractorPtr m_ref_image_sift_extractor;
        ORBFeatureExtractorPtr m_query_image_orb_extractor;
        SIFTRegionMatcherPtr m_sift_matcher;
        ORBRegionMatcherPtr m_orb_matcher;

        //ready reference image
        ImageMatchData m_ref_image;
        std::mutex m_lock_ref_image;

        //to-match reference image
        ImageMatchData m_ref_image_to_match;
        std::mutex m_lock_ref_image_to_match;

        //to-match query image
        ImageMatchData m_query_image;
        std::mutex m_lock_query_image;

        //algorithm locks
        std::mutex m_lock_indirect_match;
        std::mutex m_lock_direct_match;
        std::mutex m_orb_matcher_lock;
        std::mutex m_sift_matcher_lock;

        //ImageMatchData m_ref_image_with_matrix;
        bool m_map_ready = false;
        bool m_query_image_ready = false;
        bool m_ref_image2map_ready = false;
        bool m_ref_image2query_ready = false;
        int m_ref_image_width, m_ref_image_height;
    };
    using Image2MapIndirectPtr = std::shared_ptr<Image2MapIndirect>;

    };

//int test(){
//    Image2MapIndirect x;
//    for(int i=0; i<100; i++){
//        if(rand() % 123 == 0)
//            x.do_match_image2map();
//        x.do_match_image2map_indirect();
//        auto mat = x.get_transform_matrix();
//    }
//    return 0;
//}

}