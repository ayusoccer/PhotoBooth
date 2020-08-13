/*
 * CIS 190: C++ Programming - Project
 * http://www.cis.upenn.edu/~cis190/
 * Last Modified: 04 Dec 2019
 * Ayush Parikh (ayushp@seas.upenn.edu)
 * Dennis Gann (dgann@seas.upenn.edu)
 */

#ifndef IMAGE_ALGORITHMS_HPP
#define IMAGE_ALGORITHMS_HPP

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class ImageAlgorithms
{
public:
	static Mat cannyEdge(Mat input);
	static Mat histogramBW(Mat input);
	static Mat histogramRGB(Mat input);
	static Mat inverted(Mat input);
};

#endif
