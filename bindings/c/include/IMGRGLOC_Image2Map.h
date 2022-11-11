#pragma once

#include "IMGRGLOC_master.h"
#include "opencvc/OpenCVC.h"

extern "C" {
    struct IMGRGLOC_Image2Map;

    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_create(IMGRGLOC_Image2Map** obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_destroy(IMGRGLOC_Image2Map* obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_set_map(IMGRGLOC_Image2Map* obj, CVCMat img);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_do_map_preprocess(IMGRGLOC_Image2Map* obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_map_keypoints(IMGRGLOC_Image2Map* obj, CVCMat out_pts);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_is_map_ready(IMGRGLOC_Image2Map* obj, bool* out_is_ready);

    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_set_query_image(IMGRGLOC_Image2Map* obj, CVCMat img);

    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_do_query_image_keypoint_detection(IMGRGLOC_Image2Map* obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_do_query_image_feature_extraction(IMGRGLOC_Image2Map* obj);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_query_image_keypoints(IMGRGLOC_Image2Map* obj, CVCMat out_pts);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_is_query_image_ready(IMGRGLOC_Image2Map* obj, bool* out_is_ready);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_do_match_image2map(IMGRGLOC_Image2Map* obj, bool* out_is_success);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_transform_matrix(IMGRGLOC_Image2Map* obj, CVCMat out_transform_matrix);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_match_confidence(IMGRGLOC_Image2Map* obj, float* out_confidence);

    //memory layout: xyxyxyxyxy
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_matched_points_in_map(IMGRGLOC_Image2Map* obj, CVCMat out_points);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_matched_points_in_image(IMGRGLOC_Image2Map* obj, CVCMat out_points);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_transform_point_image2map(IMGRGLOC_Image2Map* obj, CVCMat in_point, CVCMat out_point);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_transform_points_image2map(IMGRGLOC_Image2Map* obj, CVCMat in_points, CVCMat out_points);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_transform_point_map2image(IMGRGLOC_Image2Map* obj, CVCMat in_point, CVCMat out_point);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_transform_points_map2image(IMGRGLOC_Image2Map* obj, CVCMat in_points, CVCMat out_points);
    
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_transformed_image_in_map(IMGRGLOC_Image2Map* obj, CVCMat out, int* out_top_left_x, int* out_top_left_y, CVCMat outmask);

    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_map_region_wrt_image(IMGRGLOC_Image2Map* obj, CVCMat out_polygon);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_get_map_region_wrt_map(IMGRGLOC_Image2Map* obj, CVCMat out_polygon);



    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_set_map_maxlen(IMGRGLOC_Image2Map* obj, int value);
    IMGRGLOC_ErrorCode IMGRGLOC_Image2Map_set_query_image_maxlen(IMGRGLOC_Image2Map* obj, int value);

    const char* IMGRGLOC_Image2Map_get_version();    
}