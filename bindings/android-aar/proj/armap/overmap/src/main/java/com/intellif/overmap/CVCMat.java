package com.intellif.overmap;

import android.graphics.PointF;
import android.util.Log;

import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Memory;
import com.sun.jna.ptr.PointerByReference;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.List;

/**
 * Representation of native cv::Mat
 */
public class CVCMat implements AutoCloseable {
    static {
        Native.register(CVCMat.class, "imgregionloc_c");
    }

    /**
     * create empty matrix
     */
    CVCMat(){
        m_ptr = CVCMatCreate();
    }

    /**
     * create matrix with specified size
     * @param nrow number of rows
     * @param ncol number of columns
     * @param cvtype CvType that specifies the data type of the matrix
     */
    public CVCMat(int nrow, int ncol, int cvtype){
        m_ptr = CVCMatCreateWithSize(nrow, ncol, cvtype, null);
    }

    /**
     * create matrix with specified size, wrapping external raw pixel data.
     * memory layout follows OpenCV convention (RGBRGBRGB...)
     * @param nrow number of rows
     * @param ncol number of columns
     * @param cvtype CvType that specifies the data type of the matrix
     * @param data the pixel data
     */
    public CVCMat(int nrow, int ncol, int cvtype, ByteBuffer data){
        m_ptr = CVCMatCreateWithSize(nrow, ncol, cvtype, data);
    }

    protected Pointer m_ptr;

    public Pointer get_ptr(){
        return m_ptr;
    }

//    @Override
//    protected void finalize() throws Throwable {
//        if(m_ptr!=null)
//            CVCMatFree(m_ptr);
//        m_ptr=null;
//    }

    /**
     * destroy the object explicitly
     */
    public void release(){
        if(m_ptr!=null)
            CVCMatFree(m_ptr);
        m_ptr=null;
    }

    public void set_float(int i, int j, float x){
        CVCMatSetFloat(m_ptr, i, j, x);
    }

    public float get_float(int i, int j){
        return CVCMatAtFloat(m_ptr, i, j).getFloat(0);
    }

    public byte get_byte(int i, int j){
        return CVCMatAtChar(m_ptr, i, j).getByte(0);
    }

    public void set_byte(int i, int j, byte x){
        CVCMatSetChar(m_ptr, i, j, x);
    }

    public FloatBuffer get_float_array(){
        return get_byte_array().asFloatBuffer();
    }

    public ByteBuffer get_byte_array(){
        Pointer p= CVCMatGetData(m_ptr);
        return p.getByteBuffer(0, size_in_bytes());
    }

    public int rows(){
        return CVCMatHeight(m_ptr);
    }

    public int cols(){
        return CVCMatWidth(m_ptr);
    }

    public int type_opencv(){
        return CVCMatGetType(m_ptr);
    }

    public long size_in_bytes(){
        return CVCMatGetByteCount(m_ptr);
    }

    public PointF[] to_points_float(){
        long nbytes = size_in_bytes();
        int type = type_opencv();
        assert type==CvType.CV_32F || type==CvType.CV_32FC(2): "type must be CV_32F or CV_32FC2";
        assert nbytes/4 % 2 == 0 : "matrix must contain Nx2 float numbers";

        int npts = (int)(nbytes/8);
        FloatBuffer buf = get_float_array();
        if(buf.limit() == 0)
            return null;
        PointF[] out_pts = new PointF[npts];
        for(int i=0; i<out_pts.length; i++)
            out_pts[i] = new PointF(buf.get(2*i), buf.get(2*i+1));
        return out_pts;
    }

    public void as_points_float(PointF[] pts){
        CVCMatCreateData(m_ptr, pts.length, 2, CvType.CV_32F);
        FloatBuffer buf = get_byte_array().asFloatBuffer();
        for(int i=0; i<pts.length; i++){
            buf.put(2*i, pts[i].x);
            buf.put(2*i+1,pts[i].y);
        }
    }

    public void copy_from(byte[] data){
        assert size_in_bytes()==data.length : "copying data to mat: data size mismatch";
        CVCMatCopyFrom(m_ptr, ByteBuffer.wrap(data));
    }

    public void copy_from(ByteBuffer data){
        CVCMatCopyFrom(m_ptr, data);
    }

    public static CVCMat from_jpeg_data(byte[] data){
        CVCMat out = new CVCMat();
        CVCimdecodeMat(ByteBuffer.wrap(data), data.length, CVCImreadModes.CVC_IMREAD_ANYCOLOR, out.get_ptr());
        return out;
    }

    /**
     * encode the image as jpeg stream
     * @return the jpeg raw data
     */
    public byte[] to_jpeg_data(){
        CVCMat _out = new CVCMat();
        CVCimencodeMat(".jpg", m_ptr, _out.get_ptr(), null, 0);
        byte[] out = new byte[(int)_out.size_in_bytes()];
        ByteBuffer out_buf = _out.get_byte_array();
        out_buf.get(out);
        _out.release();
        return out;
    }

    public static native Pointer CVCMatCreate();
    public static native void CVCMatFree(Pointer mat);
    public static native Pointer CVCMatCreateWithSize(int nrow, int ncol, int cvtype, ByteBuffer data);
    public static native void CVCMatCreateData(Pointer mat, int nrow, int ncol, int cvtype);
    public static native void CVCMatCopyFrom(Pointer mat, ByteBuffer data);
    public static native int CVCMatHeight(Pointer mat);
    public static native int CVCMatWidth(Pointer mat);
    public static native Pointer CVCMatGetData(Pointer mat);
    public static native Pointer CVCMatAt(Pointer mat, int nbyte_per_pixel, int i, int j);
    public static native Pointer CVCMatAtChar(Pointer mat, int i, int j);
    public static native Pointer CVCMatAtFloat(Pointer mat, int i, int j);
    public static native Pointer CVCMatAtInt(Pointer mat, int i, int j);
    public static native Pointer CVCMatAtVec3b(Pointer mat, int i, int j);
    public static native Pointer CVCMatAtVec3f(Pointer mat, int i, int j);
    public static native void CVCMatSetInt(Pointer mat, int i, int j, int data);
    public static native void CVCMatSetChar(Pointer mat, int i, int j, byte data);
    public static native void CVCMatSetFloat(Pointer mat, int i, int j, float data);
    public static native void CVCMatSetVec3b(Pointer mat, int i, int j, ByteBuffer data);
    public static native void CVCMatSetVec3f(Pointer mat, int i, int j, FloatBuffer data);
    public static native long CVCMatGetByteCount(Pointer mat);
    public static native int CVCMatGetType(Pointer mat);

    //image decoding and encoding
    public static native void CVCimdecodeMat(ByteBuffer buf, int n_byte, int imread_flags, Pointer dst);
    public static native void CVCimencodeMat(String ext, Pointer src, Pointer dst, IntBuffer params, int n_param);

    @Override
    public void close() throws Exception {
        release();
    }
}
