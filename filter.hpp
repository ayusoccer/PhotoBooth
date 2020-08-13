/*
 * CIS 190: C++ Programming - Project
 * http://www.cis.upenn.edu/~cis190/
 * Last Modified: 04 Dec 2019
 * Ayush Parikh (ayushp@seas.upenn.edu)
 * Dennis Gann (dgann@seas.upenn.edu)
 */

#ifndef FILTER_HPP
#define FILTER_HPP

#include <string>
#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"

using namespace cv;
using namespace std;

enum Feature
{
    FACE,
    EYES,
    NOSE,
    MOUTH
};

/*
 * Filter Interface (abstract class).
 */
class Filter
{
protected:
    string name;
    string spritePath;
    bool featureUsage[4];

    void overlayImage(Mat *src, Mat *overlay, const Point &location) const;

public:
    Filter(string name, string spritePath);

    string getName() const;

    bool usesFace() const;
    bool usesEyes() const;
    bool usesNose() const;
    bool usesMouth() const;

    virtual void applyFace(Mat &frame, Rect faceRegion) const = 0;
    virtual void applyEyes(Mat &frame, Rect faceRegion, Rect eyesRegion) const = 0;
    virtual void applyNose(Mat &frame, Rect faceRegion, Rect noseRegion) const = 0;
    virtual void applyMouth(Mat &frame, Rect faceRegion, Rect mouthRegion) const = 0;
};

#endif
