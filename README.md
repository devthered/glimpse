# Glimpse
### Open-Source Economic Image Processing Library
Glimpse is a C++ library that provides easy-to-use abstractions for OpenCV applications. Primarily for developers new to image processing and computer vision, glimpse allows users to setup image processing systems rapidly and conveniently on desktop and embedded (using Raspberry Pi) platforms. Glimpse is licensed under the MIT license and is free to use for personal, educational, and commercial purposes.

Glimpse provides many useful functions:

 - One-line image filtering and transformation functions
 - API for processing video using custom filters and viewing results
 - Useful debugging tools for OpenCV development, e.g. allowing the printout of matrix information
 
For example, translating an image using bare OpenCV requires the following code:

```C++
Mat input = imread("test.jpg");
int offsetx = 10;
int offsety = 15;
Mat output;
Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
warpAffine(input,output,trans_mat,input.size());
```

Using Glimpse, this transformation is trivial:

```C++
Mat input = imread("test.jpg");
Mat output;
Translate(input, output, 10, 15);
```

##Live Video Processing
Glimpse is useful for testing your own image processing alorithms in realtime. For example, to compare the processed and original video streams directly from your webcam, a user can define their own processing function:

```C++
void MyFilter(Mat &input, Mat &output)
{
  // process image
  ...
}
```

and have it run with a given framerate on their webcam video in a single line:

```C++
LiveVideoCompare(MyFilter);
```

This code will open up two windows, one streaming the original video and the other streaming the processed video.

##Quick Start

Glimpse source code is provided in the Source folder. Use the compiler of your choice to self-compile the library, or add the source code directly to your project.

##Known Issues

Glimpse is still in active development, and new features are on the way. Certain functions only support 8-bit unsigned char images, and support for floating-point images is in development.

##License

As metioned above, Glimpse is licensed under the MIT License. The terms of
the MIT license are included below:

```
Copyright (c) 2016 Glimpse Team <reed.devin@gmail.com>

Glimpse based on OpenCV, Copyright (c) 2016 Itseez

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
