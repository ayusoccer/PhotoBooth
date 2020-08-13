/*
 * CIS 190: C++ Programming - Project
 * http://www.cis.upenn.edu/~cis190/
 * Last Modified: 04 Dec 2019
 * Ayush Parikh (ayushp@seas.upenn.edu)
 * Dennis Gann (dgann@seas.upenn.edu)
 */

#ifndef DEBUG_FILTER_HPP
#define DEBUG_FILTER_HPP

#include "filter.hpp"
#include "opencv2/imgproc.hpp"

class DebugFilter : public Filter
{
public:
    DebugFilter(string name, string spritePath = "");

    void applyFace(Mat &frame, Rect faceRegion) const override;
    void applyEyes(Mat &frame, Rect faceRegion, Rect eyesRegion) const override;
    void applyNose(Mat &frame, Rect faceRegion, Rect noseRegion) const override;
    void applyMouth(Mat &frame, Rect faceRegion, Rect mouthRegion) const override;
};

#endif
