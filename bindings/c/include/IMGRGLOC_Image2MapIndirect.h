#pragma once

#include "IMGRGLOC_master.h"
#include "opencvc/OpenCVC.h"

extern "C" {
    struct IMGRGLOC_Image2MapIndirect;

    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_create(IMGRGLOC_Image2MapIndirect** obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_destroy(IMGRGLOC_Image2MapIndirect* obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_set_map(IMGRGLOC_Image2MapIndirect* obj, CVCMat img);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_do_map_preprocess(IMGRGLOC_Image2MapIndirect* obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_get_map_keypoints(IMGRGLOC_Image2MapIndirect* obj, CVCMat out_pts);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_is_map_ready(IMGRGLOC_Image2MapIndirect* obj, bool* out_is_ready);
    
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_set_ref_image(IMGRGLOC_Image2MapIndirect* obj, CVCMat img);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_do_ref_image_keypoint_detection(IMGRGLOC_Image2MapIndirect* obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_do_ref_image_feature_extraction(IMGRGLOC_Image2MapIndirect* obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_get_ref_image_keypoints_2map(IMGRGLOC_Image2MapIndirect* obj, CVCMat out_pts);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_get_ref_image_keypoints_2image(IMGRGLOC_Image2MapIndirect* obj, CVCMat out_pts);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_is_ref_image_to_map_ready(IMGRGLOC_Image2MapIndirect* obj, bool* out_is_ready);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_is_ref_image_to_query_ready(IMGRGLOC_Image2MapIndirect* obj, bool* out_is_ready);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_do_match_ref2map(IMGRGLOC_Image2MapIndirect* obj, bool* out_is_success);

    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_set_query_image(IMGRGLOC_Image2MapIndirect* obj, CVCMat img);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_do_query_image_keypoint_detection(IMGRGLOC_Image2MapIndirect* obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_do_query_image_feature_extraction(IMGRGLOC_Image2MapIndirect* obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_get_query_image_keypoints(IMGRGLOC_Image2MapIndirect* obj, CVCMat out_pts);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_is_query_image_ready(IMGRGLOC_Image2MapIndirect* obj, bool* out_is_ready);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_do_match_image2map(IMGRGLOC_Image2MapIndirect* obj, bool* out_is_success);
    
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_get_transform_matrix(IMGRGLOC_Image2MapIndirect* obj, CVCMat out_transform_matrix);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_get_match_confidence(IMGRGLOC_Image2MapIndirect* obj, float* out_confidence);

    //memory layout: xyxyxyxyxy
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_get_matched_points_in_map(IMGRGLOC_Image2MapIndirect* obj, CVCMat out_points);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_get_matched_points_in_image(IMGRGLOC_Image2MapIndirect* obj, CVCMat out_points);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_transform_point_image2map(IMGRGLOC_Image2MapIndirect* obj, CVCMat in_point, CVCMat out_point);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_transform_points_image2map(IMGRGLOC_Image2MapIndirect* obj, CVCMat in_points, CVCMat out_points);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_transform_point_map2image(IMGRGLOC_Image2MapIndirect* obj, CVCMat in_point, CVCMat out_point);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_transform_points_map2image(IMGRGLOC_Image2MapIndirect* obj, CVCMat in_points, CVCMat out_points);

    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_get_transformed_image_in_map(IMGRGLOC_Image2MapIndirect* obj, CVCMat out, int* out_top_left_x, int* out_top_left_y, CVCMat outmask);

    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_get_map_region_wrt_image(IMGRGLOC_Image2MapIndirect* obj, CVCMat out_polygon);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_get_map_region_wrt_map(IMGRGLOC_Image2MapIndirect* obj, CVCMat out_polygon);

    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_set_map_maxlen(IMGRGLOC_Image2MapIndirect* obj, int value);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_set_ref_image_maxlen(IMGRGLOC_Image2MapIndirect* obj, int value);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2MapIndirect_set_query_image_maxlen(IMGRGLOC_Image2MapIndirect* obj, int value);

    const char* IMGRGLOC_Image2MapIndirect_get_version();
}