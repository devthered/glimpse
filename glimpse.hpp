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
  public:
    // Filters
    void Sharpen(Mat &input, Mat &output, int kernelSize, double strength);
    public void Blur(Mat &input, Mat &output, int kernelSize);
    void InvertColors(Mat &input, Mat &output);
    void EqualizeHistogram(Mat &input, Mat &output);

    // Transformations
    void Translate(Mat &input, Mat &output, int offsetx, int offsety);
    public void Rotate(Mat &input, Mat &output, float radians);
    public void Scale(Mat &input, Mat &output, float scaleFactor);
    public void Scale(Mat &input, Mat &output, float scaleFactorX, float scaleFactorY);

    // Live Video Processing
    void LiveVideoCompare(double fps, void (*f)(Mat &, Mat &));

    // Debugging Helper Functions
    string type2str(int type);
    void printmat(Mat &m);
}