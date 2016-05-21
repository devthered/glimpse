//
//  glimpse.cpp
//  Glimpse
//
//  Created by Glimpse Team on 4/17/16.
//  Copyright © 2016 Glimpse Team. All rights reserved.
//

#include "glimpse.hpp"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/videoio/videoio.hpp"

using namespace cv;
using namespace std;

namespace glimpse
{
    /***********
     * Filters *
     ***********/

    // Sharpens an image with an unsharp mask subtraction technique.
    public void Sharpen(Mat &input, Mat &output, int kernelSize, double strength)
    {
        GaussianBlur(input, output, Size(0, 0), kernelSize, kernelSize);
        addWeighted(input, 1.5, output, -strength, 0, output);
    }

    // Gaussian Blurs an image symmetrically
    public void SymmetricBlur(Mat &input, Mat &output, int kernelSize)
    {
        GaussianBlur(input, output, Size(0, 0), kernelSize, kernelSize);
    }

    // Inverts color values of 3 channel uchar image.
    // TODO: add support for all matrix types
    public void InvertColors(Mat &input, Mat &output)
    {
        output = Mat::zeros(input.size(), input.type());
        
        //invert color on every pixel
        for(int i=0; i<input.rows; i++){
            for(int j=0; j<input.cols; j++){
                for(int c=0; c<3; c++){
                    output.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(255 - input.at<Vec3b>(i, j)[c]);
                }
            }
        }
    }

    // Equalizes the histogram of a color or grayscale image,
    // resolving problems with low-contrast, or very dark or washed-out images.
    public void EqualizeHistogram(Mat &input, Mat &output)
    {
        //check if the input image is valid
        if(!input.data){
            cout << "Usage: ./ProgramName ImageName" << endl;
            return;
        }
        
        //check how many channels the image has
        if(input.channels() >= 3){
           
            Mat ycrcb;
            vector<Mat> channels;
            
            cvtColor(input, ycrcb, CV_BGR2YCrCb);
            split(ycrcb, channels);
            equalizeHist(channels[0], channels[0]);
            merge(channels, ycrcb);
            cvtColor(ycrcb, output, CV_YCrCb2BGR);
            
            namedWindow("Input", CV_WINDOW_AUTOSIZE);
            namedWindow("Output", CV_WINDOW_AUTOSIZE);
            
            imshow("Input", input);
            imshow("Output", output);
            
            waitKey(0);
        }
        else{
            
            equalizeHist(input, output);
            
            namedWindow("Input", CV_WINDOW_AUTOSIZE);
            namedWindow("Output", CV_WINDOW_AUTOSIZE);
            
            imshow("Input", input);
            imshow("Output", output);
            
            waitKey(0);
        }
    }


    /*******************
     * Transformations *
     *******************/

    // Translates a matrix circularly by x and y offset values.
    public void Translate(Mat &input, Mat &output, int offsetx, int offsety)
    {
        Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
        warpAffine(input,output,trans_mat,input.size());
    }

    public void Rotate(Mat &input, Mat &output, float radians)
    {
        //TODO: implement
    }

    public void Scale(Mat &input, Mat &output, float scaleFactor)
    {
        Scale(input, output, scaleFactor, scaleFactor);
    }

    public void Scale(Mat &input, Mat &output, float scaleFactorX, float scaleFactorY)
    {
        //TODO: implement
    }

    /*******************************
     * Live Video Processing Tools *
     *******************************/

    // For comparing live video streams before and after generic processing. 
    // Users should define their own processing function that takes input and output matrices (images) as arguments.
    // This function is passed in as void (*f)(Mat &input, Mat &output), and is applied each frame to the new image coming in,
    // at a rate defined by int fps.
    // See wiki for usage examples.
    public void LiveVideoCompare(double fps, void (*f)(Mat &, Mat &))
    {
        // Connect to webcam with VideoCapture object
        VideoCapture cap(0);
        if (!cap.isOpened()) {
            std::cout << "Webcam is not open." << std::endl;
        }
        
        // Declare matrices for operation
        Mat input;
        Mat output;
        
        while (true) {
            // Get image from webCam
            cap.read(input);
            
            // Process image
            f(input,output);
            
            // Display next image
            imshow("Original", input);
            imshow("Processed", output);
            
            // Wait for frame delay, break out of video if ESC is pressed
            if(waitKey(1000.0/FPS) == 27) break;
        }
    }

    /******************************
     * Debugging Helper Functions *
     ******************************/

    // Returns the type of a matrix as a string,
    // equivalent to the name of the corresponding constant.
    // This will tell the data type and number of channels of a Mat object.
    // for Mat m, call type2str(m.type());
    public string type2str(int type)
    {
        string r;
        
        uchar depth = type & CV_MAT_DEPTH_MASK;
        uchar chans = 1 + (type >> CV_CN_SHIFT);
        
        switch ( depth ) {
            case CV_8U:  r = "8U"; break;
            case CV_8S:  r = "8S"; break;
            case CV_16U: r = "16U"; break;
            case CV_16S: r = "16S"; break;
            case CV_32S: r = "32S"; break;
            case CV_32F: r = "32F"; break;
            case CV_64F: r = "64F"; break;
            default:     r = "User"; break;
        }
        
        r += "C";
        r += (chans+'0');
        
        return r;
    }

    // For debugging only. Prints out the size and type of a given Mat object, passed by reference.
    public void printmat(Mat &m)
    {
        string ty = type2str(m.type());
        printf("Matrix: %s %dx%d \n", ty.c_str(), m.cols, m.rows);
    }
}