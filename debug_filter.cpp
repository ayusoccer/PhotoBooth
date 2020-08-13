/*
 * CIS 190: C++ Programming - Project
 * http://www.cis.upenn.edu/~cis190/
 * Last Modified: 04 Dec 2019
 * Ayush Parikh (ayushp@seas.upenn.edu)
 * Dennis Gann (dgann@seas.upenn.edu)
 */

#include "debug_filter.hpp"

DebugFilter::DebugFilter(string name, string spritePath) : Filter{name, spritePath}
{
    featureUsage[FACE] = true;
    featureUsage[EYES] = true;
    featureUsage[NOSE] = true;
    featureUsage[MOUTH] = true;
}

// concrete implementations of apply functions
void DebugFilter::applyFace(Mat &frame, Rect faceRegion) const
{
    Point center(faceRegion.x + faceRegion.width / 2, faceRegion.y + faceRegion.height / 2);
    ellipse(frame, center, Size(faceRegion.width / 2, faceRegion.height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
}

void DebugFilter::applyEyes(Mat &frame, Rect faceRegion, Rect eyesRegion) const
{
    rectangle(frame, Point(faceRegion.x + eyesRegion.x, faceRegion.y + eyesRegion.y), Point(faceRegion.x + eyesRegion.x + eyesRegion.width, faceRegion.y + eyesRegion.y + eyesRegion.height), Scalar(0, 255, 255), 4);
}

void DebugFilter::applyNose(Mat &frame, Rect faceRegion, Rect noseRegion) const
{
    rectangle(frame, Point(faceRegion.x + noseRegion.x, faceRegion.y + noseRegion.y), Point(faceRegion.x + noseRegion.x + noseRegion.width, faceRegion.y + noseRegion.y + noseRegion.height), Scalar(255, 255, 0), 4);
}

void DebugFilter::applyMouth(Mat &frame, Rect faceRegion, Rect mouthRegion) const
{
    rectangle(frame, Point(faceRegion.x + mouthRegion.x, faceRegion.y + faceRegion.height / 2 + mouthRegion.y), Point(faceRegion.x + mouthRegion.x + mouthRegion.width, faceRegion.y + faceRegion.height / 2 + mouthRegion.y + mouthRegion.height), Scalar(0, 255, 0), 4);
}
