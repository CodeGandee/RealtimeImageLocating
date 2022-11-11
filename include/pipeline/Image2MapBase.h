#pragma once

#include "imgregionloc_master.h"
#include "ImageWithFeature.h"
#include "RegionTransformer.h"

namespace imgregionloc {
    namespace pipeline {
        class Image2MapBase
        {
        public:
            virtual ~Image2MapBase() {}
            Image2MapBase() :m_map(new ImageWithFeature), m_query_image(new ImageWithFeature), m_transformer(new RegionTransformer) {}

            virtual void set_map(const ImageWithFeaturePtr& fullmap);
            virtual void set_map(const cv::Mat& img);
            virtual void set_query_image(const ImageWithFeaturePtr& query_image);
            virtual void set_query_image(const cv::Mat& img);
            virtual bool do_match_image2map();  //return true if success
            virtual cv::Mat get_transform_matrix() const;
            Point2 transform_point_image2map(const Point2& p) const;
            virtual void transform_point_image2map(const std::vector<Point2>& pts, std::vector<Point2>& out) const;
            Point2 transform_point_map2image(const Point2& p) const;
            virtual void transform_point_map2image(const std::vector<Point2>& pts, std::vector<Point2>& out) const;
            virtual void get_transformed_image_in_map(cv::Mat& out, Point2& upper_left_point, cv::Mat* out_mask = nullptr);
            virtual void get_map_region_wrt_image(Polygon2& out) const;
            virtual void get_map_region_wrt_map(Polygon2& out) const;



        protected:
            ImageWithFeaturePtr m_map;
            ImageWithFeaturePtr m_query_image;
            RegionTransformerPtr m_transformer;
            int m_image_width, m_image_height, m_map_width, m_map_height;
            Polygon2 m_image_corners;

        };
        using Image2MapBasePtr = std::shared_ptr<Image2MapBase>;
    };
}