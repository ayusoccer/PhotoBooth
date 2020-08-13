/*
 * CIS 190: C++ Programming - Project
 * http://www.cis.upenn.edu/~cis190/
 * Last Modified: 04 Dec 2019
 * Ayush Parikh (ayushp@seas.upenn.edu)
 * Dennis Gann (dgann@seas.upenn.edu)
 */

#ifndef DAPPER_FILTER_HPP
#define DAPPER_FILTER_HPP

#include "filter.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

class DapperFilter : public Filter
{
protected:
    Mat moSprite;
    Mat hatSprite;
    Mat glassesSprite;

public:
    DapperFilter(string name, string spritePath = "data/sprites/");

    void applyFace(Mat &frame, Rect faceRegion) const override;
    void applyEyes(Mat &frame, Rect faceRegion, Rect eyesRegion) const override;
    void applyNose(Mat &frame, Rect faceRegion, Rect noseRegion) const override;
    void applyMouth(Mat &frame, Rect faceRegion, Rect mouthRegion) const override;
};

#endif
