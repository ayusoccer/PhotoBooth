/*
 * CIS 190: C++ Programming - Project
 * http://www.cis.upenn.edu/~cis190/
 * Last Modified: 04 Dec 2019
 * Ayush Parikh (ayushp@seas.upenn.edu)
 * Dennis Gann (dgann@seas.upenn.edu)
 */

#include "image_algorithms.hpp"

//Canny edge detection is a technique to extract useful structural information to reduce
//the amount of data to be processed
Mat ImageAlgorithms::cannyEdge(Mat input)
{
	Mat imgGrayscale; // grayscale of input image
	Mat imgBlurred;   // intermediate blured image
	Mat imgCanny;	 // Canny edge image
	cvtColor(input, imgGrayscale, COLOR_BGR2GRAY);
	GaussianBlur(imgGrayscale, imgBlurred, Size(5, 5), 1.8);
	Canny(imgBlurred, imgCanny, 50, 100);
	return imgCanny;
}

//Histogram equalization is commonly used in image processing to enhance the contrast
//of an image by equalizing the intensity distribution
Mat ImageAlgorithms::histogramBW(Mat input)
{
	cvtColor(input, input, COLOR_BGR2GRAY);
	Mat hist_equalized_image;
	equalizeHist(input, hist_equalized_image);
	return hist_equalized_image;
}

Mat ImageAlgorithms::histogramRGB(Mat input)
{
	Mat hist_equalized_image;
	cvtColor(input, hist_equalized_image, COLOR_BGR2YCrCb);
	vector<Mat> vec_channels;
	split(hist_equalized_image, vec_channels);
	equalizeHist(vec_channels[0], vec_channels[0]);

	//Merge 3 channels in the vector to form the color image in YCrCB color space
	merge(vec_channels, hist_equalized_image);

	//Convert the histogramRGBequalized image from YCrCb to BGR color space
	cvtColor(hist_equalized_image, hist_equalized_image, COLOR_YCrCb2BGR);
	return hist_equalized_image;
}

//Image inversion flips every bit in every element basically taking the negative of the image
Mat ImageAlgorithms::inverted(Mat input)
{
	Mat invertedImage;
	bitwise_not(input, invertedImage);
	return invertedImage;
}
