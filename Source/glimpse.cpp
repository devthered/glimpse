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
    void Sharpen(Mat &input, Mat &output, int kernelSize, double strength)
    {
        GaussianBlur(input, output, Size(0, 0), kernelSize, kernelSize);
        addWeighted(input, 1 + strength, output, -strength, 0, output);
    }

    // Gaussian Blurs an image symmetrically
    void SymmetricBlur(Mat &input, Mat &output, int kernelSize)
    {
        GaussianBlur(input, output, Size(0, 0), kernelSize, kernelSize);
    }

    // Inverts color values of 3 channel uchar image.
    // TODO: add support for all matrix types
    // TODO: use numpy to optimize
    void InvertColors(Mat &input, Mat &output)
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
    // resolving problems with low-contrast, or very dark or washed-out images
    void EqualizeHistogram(Mat &input, Mat &output)
    {
        if(input.channels() >= 3)
        {
            Mat ycrcb;
            vector<Mat> channels;
            cvtColor(input, ycrcb, CV_BGR2YCrCb);
            split(ycrcb, channels);
            equalizeHist(channels[0], channels[0]);
            merge(channels, ycrcb);
            cvtColor(ycrcb, output, CV_YCrCb2BGR);
        }
        else if (input.channels() == 1) 
        {
            equalizeHist(input, output);
        }
    }


    /*******************
     * Transformations *
     *******************/

    // Translates a matrix circularly by x and y offset values.
    void Translate(Mat &input, Mat &output, int offsetx, int offsety)
    {
        Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
        warpAffine(input, output, trans_mat, input.size());
    }

    // Rotates an image counterclockwise by a number of degrees around a given point. 
    // Preserves the size of the original image. Center coordinates are in pixel coords.
    void RotateDeg(Mat &input, Mat &output, double degrees, int centerX, int centerY)
    {
        Mat rotate_mat = getRotationMatrix2D(*new Point2f(centerX, centerY), degrees, 1);
        warpAffine(input, output, rotate_mat, input.size());
    }

    // Rotates an image counterclockwise by a number of degrees. Preserves the size of the original image.
    // Rotates about center point on default.
    void RotateDeg(Mat &input, Mat &output, double degrees)
    {
        RotateDeg(input, output, degrees, input.cols / 2, input.rows / 2);
    }

    // Rotates an image counterclockwise by a number of radians around a given point. 
    // Preserves the size of the original image. Center coordinates are in pixel coords.
    void RotateRad(Mat &input, Mat &output, float radians, int centerX, int centerY)
    {
        Mat rotate_mat = getRotationMatrix2D(*new Point2f(centerX, centerY), (double)(radians * 180.0 / 3.1415926), 1);
        warpAffine(input, output, rotate_mat, input.size());
    }

    // Rotates an image counterclockwise by a number of radians. Preserves the size of the original image.
    // Rotates about center point on default.
    void RotateRad(Mat &input, Mat &output, float radians)
    {
        RotateRad(input, output, radians, input.cols / 2, input.rows / 2);
    }

    // Scales an image asymmetrically by x and y scale factors
    void Scale(Mat &input, Mat &output, double scaleFactorX, double scaleFactorY)
    {
        Mat scale_mat = (Mat_<double>(2,3) << scaleFactorX, 0, 0, 0, scaleFactorY, 0);
        warpAffine(input, output, scale_mat, input.size());
    }

    // Scales an image symmetrically by scaleFactor
    void Scale(Mat &input, Mat &output, double scaleFactor)
    {
        Scale(input, output, scaleFactor, scaleFactor);
    }

    // Shears an image in x and y directions
    void Shear(Mat &input, Mat &output, double shearX, double shearY)
    {
        Mat shear_mat = (Mat_<double>(2,3) << 1, shearX, 0, shearY, 1, 0);
        warpAffine(input, output, shear_mat, input.size());
    }

    // Shears an image in the x direction by shearFactor
    void ShearX(Mat &input, Mat &output, double shearFactor)
    {
        Shear(input, output, shearFactor, 0);
    }

    // Shears an image in the x direction by shearFactor
    void ShearY(Mat &input, Mat &output, double shearFactor)
    {
        Shear(input, output, 0, shearFactor);
    }

    /*******************************
     * Live Video Processing Tools *
     *******************************/

    // For comparing live video streams before and after generic processing. 
    // Users should define their own processing function that takes input and output matrices (images) as arguments.
    // This function is passed in as void (*f)(Mat &input, Mat &output), and is applied each frame to the new image coming in,
    // at a rate defined by int fps.
    // See wiki for usage examples.
    void LiveVideoCompare(double fps, void (*f)(Mat &, Mat &))
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
            if(waitKey(1000.0/fps) == 27) break;
        }
    }

    // An overload with default framerate of 24 frames per second
    void LiveVideoCompare(void (*f)(Mat &, Mat &))
    {
        LiveVideoCompare(24.0, f);
    }

    /****************
     * Seam Carving *
     ****************/

     // Produces energy map for seam carving
    void GradientSquaredEnergyMap(Mat &input, Mat &output)
    {
        Mat converted;
        input.convertTo(converted, CV_32FC3, 1/255.0);
        Mat trans1, trans2, temp;
        Translate(converted, trans1, 1, 0);
        Translate(converted, trans2, 0, 1);
        absdiff(converted, trans1, temp);
        absdiff(converted, trans2, converted);
        add(converted, temp, converted);
        converted.mul(converted);
        cvtColor(converted, output, CV_RGB2GRAY);
    }
    
    float FindVerticalSeam(int* seam, Mat energyMap)
    {
        Mat map = energyMap;
        
        int rows = map.rows;
        int cols = map.cols;
        
        int *bestSeam = new int[rows];
        int *currentSeam = new int[rows];
        float lowestTotalEnergy, currentCost, totalCurrentCost;
        int index;
        lowestTotalEnergy = MAXFLOAT;
        
        for (int j = 0; j < cols; j++)
        {
            currentSeam[0] = j;
            totalCurrentCost = 0;
            for (int i = 1; i < rows; i++)
            {
                index = currentSeam[i-1];
                currentCost = energyMap.at<float>(i, index);
                currentSeam[i] = index;
                
                if (index > 2 && energyMap.at<float>(i, index-1) < currentCost)
                {
                    currentCost = energyMap.at<float>(i, index-1);
                    currentSeam[i] = index-1;
                }
                
                if (index < cols-1 && energyMap.at<float>(i, index+1) < currentCost)
                {
                    currentCost = energyMap.at<float>(i, index+1);
                    currentSeam[i] = index+1;
                }
                
                totalCurrentCost += currentCost;
                if (totalCurrentCost > lowestTotalEnergy)
                    break;
            }
            
            if (totalCurrentCost < lowestTotalEnergy)
            {
                memcpy(bestSeam, currentSeam, sizeof(int) * rows);
                lowestTotalEnergy = totalCurrentCost;
            }
        }
        
        memcpy(seam, bestSeam, sizeof(int) * rows);
        return lowestTotalEnergy;
    }
    
    float FindHorizontalSeam(int* seam, Mat energyMap)
    {
        Mat map = energyMap;
        
        int rows = map.rows;
        int cols = map.cols;
        
        int *bestSeam = new int[cols];
        int *currentSeam = new int[cols];
        float lowestTotalEnergy, currentCost, totalCurrentCost;
        int index;
        lowestTotalEnergy = MAXFLOAT;
        
        for (int i = 0; i < rows; i++)
        {
            currentSeam[0] = i;
            totalCurrentCost = 0;
            for (int j = 1; j < cols; j++)
            {
                index = currentSeam[j-1];
                currentCost = energyMap.at<float>(index, j);
                currentSeam[j] = index;
                
                if (index > 2 && energyMap.at<float>(index-1, j) < currentCost)
                {
                    currentCost = energyMap.at<float>(index-1, j);
                    currentSeam[j] = index-1;
                }
                
                if (index < rows-1 && energyMap.at<float>(index+1, j) < currentCost)
                {
                    currentCost = energyMap.at<float>(index+1, j);
                    currentSeam[j] = index+1;
                }
                
                totalCurrentCost += currentCost;
                if (totalCurrentCost > lowestTotalEnergy)
                    break;
            }
            
            if (totalCurrentCost < lowestTotalEnergy)
            {
                memcpy(bestSeam, currentSeam, sizeof(int) * cols);
                lowestTotalEnergy = totalCurrentCost;
            }
        }
        
        memcpy(seam, bestSeam, sizeof(int) * cols);
        return lowestTotalEnergy;
    }
    
    void RemoveVerticalSeams(Mat &input, Mat &output, int seamsToRemove)
    {
        Mat energyMap, temp;
        output = input.clone();
        int rows = input.rows;
        int cols = input.cols;
        
        for (int x = 0; x < seamsToRemove; x++)
        {
            int * seam = (int*)malloc(rows * sizeof(int));
            GradientSquaredEnergyMap(output, energyMap);
            FindVerticalSeam(seam, energyMap);
            
            temp = Mat::zeros(rows, cols-1, CV_8UC3);
            for (int j = 0; j < rows; j++)
            {
                int index = seam[j];
                for (int i = 0; i < index; i++)
                    temp.at<Vec3b>(j, i) = output.at<Vec3b>(j, i);
                
                for (int i = index + 1; i < cols; i++)
                    temp.at<Vec3b>(j, i-1) = output.at<Vec3b>(j, i);
            }
            
            output = temp;
            cols--;
        }
    }
    
    void RemoveHorizontalSeams(Mat &input, Mat &output, int seamsToRemove)
    {
        Mat energyMap, temp;
        output = input.clone();
        int rows = input.rows;
        int cols = input.cols;
        
        for (int x = 0; x < seamsToRemove; x++)
        {
            int * seam = (int*)malloc(cols * sizeof(int));
            GradientSquaredEnergyMap(output, energyMap);
            FindHorizontalSeam(seam, energyMap);
            
            temp = Mat::zeros(rows-1, cols, CV_8UC3);
            for (int i = 0; i < cols; i++)
            {
                int index = seam[i];
                for (int j = 0; j < index; j++)
                    temp.at<Vec3b>(j, i) = output.at<Vec3b>(j, i);
                
                for (int j = index + 1; j < rows; j++)
                    temp.at<Vec3b>(j-1, i) = output.at<Vec3b>(j, i);
            }
            
            output = temp;
            rows--;
        }
    }
    
    void Retarget(Mat &input, Mat &output, int newWidth, int newHeight)
    {
        int rows = input.rows;
        int cols = input.cols;
        
        float currentAspectRatio = (double)cols / rows;
        float newAspectRatio = (double)newWidth / newHeight;
        
        if (newAspectRatio > currentAspectRatio) //make wider, remove horizontal seams
        {
            int rowsToRemove = rows - (int)(cols / newAspectRatio);
            RemoveHorizontalSeams(input, output, rowsToRemove);
            resize(output, output, Size(newWidth, newHeight));
        }
        else if (newAspectRatio < currentAspectRatio) //make taller, remove vertical seams
        {
            int colsToRemove = cols - (int)(rows * newAspectRatio);
            RemoveVerticalSeams(input, output, colsToRemove);
            resize(output, output, Size(newWidth, newHeight));
        }
        else //do not retarget, simply resize the image normally
        {
            resize(input, output, Size(newWidth, newHeight));
        }
    }

    /******************************
     * Debugging Helper Functions *
     ******************************/

    // Returns the type of a matrix as a string,
    // equivalent to the name of the corresponding constant.
    // This will tell the data type and number of channels of a Mat object.
    // for Mat m, call type2str(m.type());
    string type2str(int type)
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
    void printmat(Mat &m)
    {
        string ty = type2str(m.type());
        printf("Matrix: %s %dx%d \n", ty.c_str(), m.cols, m.rows);
    }
}