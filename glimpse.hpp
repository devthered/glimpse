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
    void Sharpen(Mat &input, Mat &output, double strength);
    void Translate(Mat &input, Mat &output, int offsetx, int offsety);
    void InvertColors(Mat &input, Mat &output);

    // Live Video Processing
    void LiveVideoCompare(double fps, void (*f)(Mat &, Mat &));

    // Debugging Helper Functions
    string type2str(int type);
    void printmat(Mat &m);
}