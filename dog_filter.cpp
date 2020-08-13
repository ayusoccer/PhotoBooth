/*
 * CIS 190: C++ Programming - Project
 * http://www.cis.upenn.edu/~cis190/
 * Last Modified: 04 Dec 2019
 * Ayush Parikh (ayushp@seas.upenn.edu)
 * Dennis Gann (dgann@seas.upenn.edu)
 */

#include "dog_filter.hpp"

DogFilter::DogFilter(string name, string spritePath) : Filter{name, spritePath}
{
    featureUsage[FACE] = false;
    featureUsage[EYES] = true;
    featureUsage[NOSE] = true;
    featureUsage[MOUTH] = true;
    mouthSprite = imread(spritePath + "dog_tongue.png", IMREAD_UNCHANGED);
    noseSprite = imread(spritePath + "dog_nose.png", IMREAD_UNCHANGED);
}

// concrete implementations of apply functions
void DogFilter::applyFace(Mat &frame, Rect faceRegion) const
{
}

void DogFilter::applyEyes(Mat &frame, Rect faceRegion, Rect eyesRegion) const
{
}

void DogFilter::applyNose(Mat &frame, Rect faceRegion, Rect noseRegion) const
{
    Size noseSize(noseRegion.height * 1.7, noseRegion.height);
    Mat nose;
    resize(noseSprite, nose, noseSize);
    int xOffset = (noseRegion.height * 1.7 - noseRegion.width) / 2;
    overlayImage(&frame, &nose, Point(faceRegion.x + noseRegion.x - xOffset, faceRegion.y + noseRegion.y));
}

void DogFilter::applyMouth(Mat &frame, Rect faceRegion, Rect mouthRegion) const
{
    Size tongueSize(mouthRegion.width, mouthRegion.width * 1.16);
    Mat tongue;
    resize(mouthSprite, tongue, tongueSize);
    overlayImage(&frame, &tongue, Point(faceRegion.x + mouthRegion.x, faceRegion.y + faceRegion.height / 2 + mouthRegion.y));
}
