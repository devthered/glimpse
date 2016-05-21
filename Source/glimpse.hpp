//
//  glimpse.hpp
//  Glimpse
//
//  Created by Glimpse Team on 4/17/16.
//  Copyright © 2016 Glimpse Team. All rights reserved.
//

#ifndef GLIMPSE_HPP_
#define GLIMPSE_HPP_
#endif

namespace glimpse 
{
    // Filters
    void Sharpen(Mat &input, Mat &output, int kernelSize, double strength);
    void Blur(Mat &input, Mat &output, int kernelSize);
    void InvertColors(Mat &input, Mat &output);
    void EqualizeHistogram(Mat &input, Mat &output);

    // Transformations
    void Translate(Mat &input, Mat &output, int offsetx, int offsety);
    void RotateDeg(Mat &input, Mat &output, double degrees, int centerX, int centerY);
    void RotateDeg(Mat &input, Mat &output, double degrees);
    void RotateRad(Mat &input, Mat &output, float radians, int centerX, int centerY);
    void RotateRad(Mat &input, Mat &output, float radians);
    void Scale(Mat &input, Mat &output, double scaleFactorX, double scaleFactorY);
    void Scale(Mat &input, Mat &output, double scaleFactor);
    void Shear(Mat &input, Mat &output, double shearX, double shearY);
    void ShearX(Mat &input, Mat &output, double shearFactor);
    void ShearY(Mat &input, Mat &output, double shearFactor);
    
    // Live Video Processing Tools
    void LiveVideoCompare(double fps, void (*f)(Mat &, Mat &));

    // Debugging Helper Functions
    string type2str(int type);
    void printmat(Mat &m);
}