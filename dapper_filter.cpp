/*
 * CIS 190: C++ Programming - Project
 * http://www.cis.upenn.edu/~cis190/
 * Last Modified: 04 Dec 2019
 * Ayush Parikh (ayushp@seas.upenn.edu)
 * Dennis Gann (dgann@seas.upenn.edu)
 */

#include "dapper_filter.hpp"

DapperFilter::DapperFilter(string name, string spritePath) : Filter{name, spritePath}
{
    featureUsage[FACE] = true;
    featureUsage[EYES] = true;
    featureUsage[NOSE] = false;
    featureUsage[MOUTH] = true;
    moSprite = imread(spritePath + "mo.png", IMREAD_UNCHANGED);
    hatSprite = imread(spritePath + "hat.png", IMREAD_UNCHANGED);
    glassesSprite = imread(spritePath + "glasses.png", IMREAD_UNCHANGED);
}

// concrete implementations of apply functions
void DapperFilter::applyFace(Mat &frame, Rect faceRegion) const
{
    Size hatSize(faceRegion.width, faceRegion.width * 0.6);
    Mat hat;
    resize(hatSprite, hat, hatSize);
    int yPos = faceRegion.y - hatSize.height;
    if (yPos < 0)
        yPos = 0;
    overlayImage(&frame, &hat, Point(faceRegion.x, yPos));
}

void DapperFilter::applyEyes(Mat &frame, Rect faceRegion, Rect eyesRegion) const
{
    Size shadesSize(eyesRegion.width * 1.2, eyesRegion.width * 0.5);
    Mat shades;
    resize(glassesSprite, shades, shadesSize);
    int xOffset = (eyesRegion.width * 1.2 - eyesRegion.width) / 2;
    overlayImage(&frame, &shades, Point(faceRegion.x + eyesRegion.x - xOffset, faceRegion.y + eyesRegion.y - shadesSize.height / 4));
}

void DapperFilter::applyNose(Mat &frame, Rect faceRegion, Rect noseRegion) const
{
}

void DapperFilter::applyMouth(Mat &frame, Rect faceRegion, Rect mouthRegion) const
{
    Size moSize(mouthRegion.height * 2.5, mouthRegion.height);
    Mat mo;
    resize(moSprite, mo, moSize);
    int xOffset = (mouthRegion.height * 2.5 - mouthRegion.width) / 2;
    overlayImage(&frame, &mo, Point(faceRegion.x + mouthRegion.x - xOffset, faceRegion.y + faceRegion.height / 2 + mouthRegion.y - moSize.height / 2));
}
