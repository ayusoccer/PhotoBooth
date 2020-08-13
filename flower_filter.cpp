/*
 * CIS 190: C++ Programming - Project
 * http://www.cis.upenn.edu/~cis190/
 * Last Modified: 04 Dec 2019
 * Ayush Parikh (ayushp@seas.upenn.edu)
 * Dennis Gann (dgann@seas.upenn.edu)
 */

#include "flower_filter.hpp"

FlowerFilter::FlowerFilter(string name, string spritePath) : Filter{name, spritePath}
{
    featureUsage[FACE] = true;
    featureUsage[EYES] = true;
    featureUsage[NOSE] = false;
    featureUsage[MOUTH] = false;
    sunflowerSprite = imread(spritePath + "sunflower.png", IMREAD_UNCHANGED);
    crownSprite = imread(spritePath + "flower_crown.png", IMREAD_UNCHANGED);
}

// concrete implementations of apply functions
void FlowerFilter::applyFace(Mat &frame, Rect faceRegion) const
{
    Size crownSize(faceRegion.width, faceRegion.height / 3);
    Mat crown;
    resize(crownSprite, crown, crownSize);
    int yPos = faceRegion.y - faceRegion.height / 3;
    if (yPos < 0)
        yPos = 0;
    overlayImage(&frame, &crown, Point(faceRegion.x, yPos));
}

void FlowerFilter::applyEyes(Mat &frame, Rect faceRegion, Rect eyesRegion) const
{
    Size sunflowerSize(eyesRegion.height * 1.5, eyesRegion.height * 1.5);
    Mat sunflower;
    resize(sunflowerSprite, sunflower, sunflowerSize);
    overlayImage(&frame, &sunflower, Point(faceRegion.x + eyesRegion.x, faceRegion.y + eyesRegion.y));
    overlayImage(&frame, &sunflower, Point(faceRegion.x + eyesRegion.x + eyesRegion.width - sunflowerSize.width, faceRegion.y + eyesRegion.y));
}

void FlowerFilter::applyNose(Mat &frame, Rect faceRegion, Rect noseRegion) const
{
}

void FlowerFilter::applyMouth(Mat &frame, Rect faceRegion, Rect mouthRegion) const
{
}
