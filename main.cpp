//
//  main.cpp
//  Glimpse
//
//  Created by Devin Reed on 4/17/16.
//  Copyright © 2016 Devin Reed. All rights reserved.
//

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/videoio/videoio.hpp"

using namespace cv;

namespace glimpse
{
    
}

int main() {
    VideoCapture cap(0);
    
    if (!cap.isOpened()) {
        std::cout << "Webcam is not open." << std::endl;
    }
    
    double FPS = 24.0;
    Mat input;
    Mat output;
    
    while (true) {
        cap.read(input);
        GaussianBlur(input, output, Size(0, 0), 4, 4);
        addWeighted(input, 1.5, output, -0.8, 0, output);
        
        imshow("Original", input);
        imshow("Sharpened", output);
        
        if(waitKey(1000.0/FPS) == 27) break;
    }
    
    return 0;
}
