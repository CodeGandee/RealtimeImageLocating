package com.intellif.overmap;

import android.graphics.PointF;
import android.provider.ContactsContract;
import android.util.Log;

import com.sun.jna.Library;
import com.sun.jna.Memory;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.ByteByReference;
import com.sun.jna.ptr.FloatByReference;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class Image2Map implements AutoCloseable{
    static{
        Native.register(Image2Map.class, "imgregionloc_c");
    }

    public Image2Map(){
        PointerByReference p = new PointerByReference();
        IMGRGLOC_Image2Map_create(p);
        m_ptr = p.getValue();
    }

    //actual data pointer
    protected Pointer m_ptr;

    public Pointer get_ptr(){
        return m_ptr;
    }

    @Override
    public void close() throws Exception {
        release();
    }

    /**
     * destroy the object explicitly
     */
    public void release(){
        if(m_ptr!=null)
            IMGRGLOC_Image2Map_destroy(m_ptr);
        m_ptr=null;
    }

    /**
     * set the background map
     * @param map the large background map
     */
    public void set_map(CVCMat map){
        IMGRGLOC_Image2Map_set_map(m_ptr, map.get_ptr());
    }

    /**
     * extract map features and store them
     */
    public void do_map_preprocess(){
        IMGRGLOC_Image2Map_do_map_preprocess(m_ptr);
    }

    public boolean is_map_ready(){
        ByteByReference p = new ByteByReference();
        IMGRGLOC_Image2Map_is_map_ready(m_ptr, p);
        return p.getValue() > 0;
    }

    public void set_query_image(CVCMat img){
        IMGRGLOC_Image2Map_set_query_image(m_ptr, img.get_ptr());
    }

    public void do_query_image_keypoint_detection(){
        IMGRGLOC_Image2Map_do_query_image_keypoint_detection(m_ptr);
    }

    public void do_query_image_feature_extraction(){
        IMGRGLOC_Image2Map_do_query_image_feature_extraction(m_ptr);
    }

    public boolean is_query_image_ready(){
        ByteByReference p = new ByteByReference();
        IMGRGLOC_Image2Map_is_query_image_ready(m_ptr, p);
        return p.getValue()>0;
    }

    public boolean do_match_image2map(){
        ByteByReference p = new ByteByReference();
        IMGRGLOC_Image2Map_do_match_image2map(m_ptr, p);
        return p.getValue()>0;
    }

    public CVCMat get_transform_matrix(){
        CVCMat out = new CVCMat();
        IMGRGLOC_Image2Map_get_transform_matrix(m_ptr, out.get_ptr());
        return out;
    }

    public float get_match_confidence(){
        FloatByReference p = new FloatByReference();
        IMGRGLOC_Image2Map_get_match_confidence(m_ptr, p);
        return p.getValue();
    }

    public CVCMat get_matched_points_in_map(){
        CVCMat out = new CVCMat();
        IMGRGLOC_Image2Map_get_matched_points_in_map(m_ptr, out.get_ptr());
        return out;
    }

    public CVCMat get_matched_points_in_image(){
        CVCMat out = new CVCMat();
        IMGRGLOC_Image2Map_get_matched_points_in_image(m_ptr, out.get_ptr());
        return out;
    }

    public PointF transform_point_image2map(PointF p){
        PointF[] pts_in = new PointF[1];
        pts_in[0] = p;
        PointF[] pts_out = transform_point_image2map(pts_in);
        return pts_out[0];
    }

    public PointF[] transform_point_image2map(PointF[] pts){
        CVCMat mat_in = new CVCMat();
        mat_in.as_points_float(pts);
        CVCMat mat_out = new CVCMat();
        IMGRGLOC_Image2Map_transform_points_image2map(m_ptr, mat_in.get_ptr(), mat_out.get_ptr());
        PointF[] out = mat_out.to_points_float();

        mat_in.release();
        mat_out.release();
        return out;
    }

    public PointF transform_point_map2image(PointF p){
        PointF[] pts_in = new PointF[1];
        pts_in[0] = p;
        PointF[] pts_out = transform_point_map2image(pts_in);
        return pts_out[0];
    }

    public PointF[] transform_point_map2image(PointF[] pts){
        CVCMat mat_in = new CVCMat();
        mat_in.as_points_float(pts);
        CVCMat mat_out = new CVCMat();
        IMGRGLOC_Image2Map_transform_points_map2image(m_ptr, mat_in.get_ptr(), mat_out.get_ptr());
        PointF[] out= mat_out.to_points_float();
        mat_in.release();
        mat_out.release();
        return out;
    }

    public PointF[] get_map_region_in_image(){
        CVCMat mat_out = new CVCMat();
        IMGRGLOC_Image2Map_get_map_region_wrt_image(m_ptr, mat_out.get_ptr());
        PointF[] out = mat_out.to_points_float();
        mat_out.release();
        return out;
    }

    public PointF[] get_map_region_in_map(){
        CVCMat mat_out = new CVCMat();
        IMGRGLOC_Image2Map_get_map_region_wrt_map(m_ptr, mat_out.get_ptr());
        PointF[] out = mat_out.to_points_float();
        mat_out.release();
        return out;
    }

    public PointF[] get_map_keypoints(){
        CVCMat mat = new CVCMat();
        IMGRGLOC_Image2Map_get_map_keypoints(m_ptr, mat.get_ptr());
        PointF[] pts = mat.to_points_float();
        mat.release();
        return pts;
    }

    public PointF[] get_query_image_keypoints(){
        CVCMat mat = new CVCMat();
        IMGRGLOC_Image2Map_get_query_image_keypoints(m_ptr, mat.get_ptr());
        PointF[] pts = mat.to_points_float();
        mat.release();
        return pts;
    }

    public void set_map_longer_edge_limit(int npixel){
        IMGRGLOC_Image2Map_set_map_maxlen(m_ptr, npixel);
    }

    public void set_query_image_longer_edge_limit(int npixel){
        IMGRGLOC_Image2Map_set_query_image_maxlen(m_ptr, npixel);
    }

    public String get_version(){
        return IMGRGLOC_Image2Map_get_version();
    }

    public static native Pointer IMGRGLOC_TEST_allocate_random_string(int n);
    public static native void IMGRGLOC_TEST_free_string(Pointer s);
    public static native void IMGRGLOC_TEST_random_bytes(ByteBuffer buf, int n);

    public static native int IMGRGLOC_Image2Map_create(PointerByReference obj);
    public static native int IMGRGLOC_Image2Map_destroy(Pointer obj);
    public static native int IMGRGLOC_Image2Map_set_map(Pointer obj, Pointer img);
    public static native int IMGRGLOC_Image2Map_do_map_preprocess(Pointer obj);
    public static native int IMGRGLOC_Image2Map_is_map_ready(Pointer obj, ByteByReference out_is_ready);
    public static native int IMGRGLOC_Image2Map_set_query_image(Pointer obj, Pointer img);
    public static native int IMGRGLOC_Image2Map_do_query_image_keypoint_detection(Pointer obj);
    public static native int IMGRGLOC_Image2Map_do_query_image_feature_extraction(Pointer obj);
    public static native int IMGRGLOC_Image2Map_is_query_image_ready(Pointer obj, ByteByReference out_is_ready);
    public static native int IMGRGLOC_Image2Map_do_match_image2map(Pointer obj, ByteByReference out_is_success);
    public static native int IMGRGLOC_Image2Map_get_transform_matrix(Pointer obj, Pointer out_transform_matrix);
    public static native int IMGRGLOC_Image2Map_get_match_confidence(Pointer obj, FloatByReference out_confidence);
    public static native int IMGRGLOC_Image2Map_get_matched_points_in_map(Pointer obj, Pointer out_points);
    public static native int IMGRGLOC_Image2Map_get_matched_points_in_image(Pointer obj, Pointer out_points);
    public static native int IMGRGLOC_Image2Map_transform_point_image2map(Pointer obj, Pointer in_point, Pointer out_point);
    public static native int IMGRGLOC_Image2Map_transform_points_image2map(Pointer obj, Pointer in_points, Pointer out_points);
    public static native int IMGRGLOC_Image2Map_transform_point_map2image(Pointer obj, Pointer in_point, Pointer out_point);
    public static native int IMGRGLOC_Image2Map_transform_points_map2image(Pointer obj, Pointer in_points, Pointer out_points);
    public static native int IMGRGLOC_Image2Map_get_transformed_image_in_map(Pointer obj, Pointer out, IntByReference out_top_left_x, IntByReference out_top_left_y, Pointer outmask);
    public static native int IMGRGLOC_Image2Map_get_map_region_wrt_image(Pointer obj, Pointer out_polygon);
    public static native int IMGRGLOC_Image2Map_get_map_region_wrt_map(Pointer obj, Pointer out_polygon);
    public static native int IMGRGLOC_Image2Map_get_map_keypoints(Pointer obj, Pointer out_points);
    public static native int IMGRGLOC_Image2Map_get_query_image_keypoints(Pointer obj, Pointer out_pts);

    public static native int IMGRGLOC_Image2Map_set_map_maxlen(Pointer obj, int value);
    public static native int IMGRGLOC_Image2Map_set_query_image_maxlen(Pointer obj, int value);

    public static native String IMGRGLOC_Image2Map_get_version();


}
