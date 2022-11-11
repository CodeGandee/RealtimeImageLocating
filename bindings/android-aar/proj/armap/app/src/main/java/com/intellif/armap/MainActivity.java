package com.intellif.armap;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.PointF;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.intellif.overmap.*;
import com.sun.jna.Memory;
import com.sun.jna.Pointer;

import org.apache.commons.io.IOUtil;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;
import org.opencv.imgcodecs.Imgcodecs;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    protected CVCMat loadImageFromAsset(String filename, List<Mat> ref_store) throws IOException{
        InputStream ims = getAssets().open(filename);
        ByteBuffer buf_input = ByteBuffer.allocateDirect(ims.available());
        Channels.newChannel(ims).read(buf_input);

        Mat rawdata = new Mat(1, buf_input.limit(), org.opencv.core.CvType.CV_8UC1, buf_input);
        Mat img = Imgcodecs.imdecode(rawdata, Imgcodecs.IMREAD_ANYCOLOR);
        rawdata.release();

        Pointer p = new Pointer(img.dataAddr());
        ByteBuffer buf_output = p.getByteBuffer(0, img.total() * img.elemSize());
        ref_store.add(img);

        CVCMat out = new CVCMat(img.rows(), img.cols(), img.type(), buf_output);
        return out;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tx = findViewById(R.id.mytext);

        if(!OpenCVLoader.initDebug()){
            throw new RuntimeException("failed to load opencv");
        }

        Image2Map obj = new Image2Map();

        tx.setText(obj.get_version());
        Log.i("armap", obj.get_version());

        ArrayList<Mat> store = new ArrayList<Mat>();
        try {
            CVCMat bgmap = loadImageFromAsset("largemap.jpg", store);
            if(true)
            {
                InputStream ims = getAssets().open("largemap.jpg");
                byte[] imgdata = IOUtil.toByteArray(ims);
                bgmap = CVCMat.from_jpeg_data(imgdata);
            }

            CVCMat query_image = loadImageFromAsset("test_imgs/001.jpg",store);
            if(true)
            {
                InputStream ims = getAssets().open("test_imgs/001.jpg");
                byte[] imgdata = IOUtil.toByteArray(ims);
                query_image = CVCMat.from_jpeg_data(imgdata);
            }

            //detect map keypoints
            obj.set_map(bgmap);
            obj.do_map_preprocess();
            Log.i("armap", "map preprocess done");
            if(false)
            {
                //print some keypoints
                PointF[] keypts = obj.get_map_keypoints();
                for(int i=0; i<keypts.length && i<10; i++)
                    Log.i("armap", keypts[i].toString());
            }

            //detect image keypoints
            obj.set_query_image_longer_edge_limit(720);
            obj.set_query_image(query_image);
            obj.do_query_image_keypoint_detection();
            obj.do_query_image_feature_extraction();
            if(false)
            {
                //print some keypoints
                PointF[] keypts = obj.get_query_image_keypoints();
                Log.i("armap","query image keypoints");
                for(int i=0; i<keypts.length && i<10; i++)
                    Log.i("armap", keypts[i].toString());
            }

            //match
            obj.do_match_image2map();
            {
                Log.i("armap/match_result", String.format("matched done, confidence = %f", obj.get_match_confidence()));
                PointF[] keypts_query = obj.get_matched_points_in_image().to_points_float();
                PointF[] keypts_map = obj.get_matched_points_in_map().to_points_float();
                for(int i=0; i<keypts_query.length && i<10; i++){
                    PointF p = keypts_query[i];
                    PointF q = keypts_map[i];
                    Log.i("armap/match_result",String.format("(%f, %f) -> (%f, %f)", p.x, p.y, q.x, q.y));
                }
            }

            //transform points between map and image
            {
                Log.i("armap/transform", "checking transformation");
                PointF image_point = new PointF(10,20);
                PointF image_point_in_map = obj.transform_point_image2map(image_point);

                PointF map_point = image_point_in_map;
                PointF map_point_in_image = obj.transform_point_map2image(map_point);

                Log.i("armap/transform", String.format("image to map %s->%s", image_point, image_point_in_map));
                Log.i("armap/transform", String.format("map to image %s->%s", map_point, map_point_in_image));
            }

            //region
            {
                Log.i("armap/region", "checking regions");
                PointF[] pts_image_in_map = obj.get_map_region_in_map();
                PointF[] pts_map_in_image = obj.get_map_region_in_image();
                Log.i("armap/region", "image in map");
                for(int i=0; i<pts_image_in_map.length; i++)
                    Log.i("armap/region", String.format("%s", pts_image_in_map[i].toString()));

                Log.i("armap/region", "map in image");
                for(int i=0; i<pts_image_in_map.length; i++)
                    Log.i("armap/region", String.format("%s", pts_map_in_image[i].toString()));
            }

            bgmap.release();
            query_image.release();

        } catch (IOException e) {
            e.printStackTrace();
        }

        obj.release();
    }
}