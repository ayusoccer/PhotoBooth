/*
 * CIS 190: C++ Programming - Project
 * http://www.cis.upenn.edu/~cis190/
 * Last Modified: 04 Dec 2019
 * Ayush Parikh (ayushp@seas.upenn.edu)
 * Dennis Gann (dgann@seas.upenn.edu)
 */

#include "filter.hpp"

Filter::Filter(string name, string spritePath) : name{name}, spritePath{spritePath}
{
}

string Filter::getName() const
{
    return name;
}

bool Filter::usesFace() const
{
    return featureUsage[FACE];
}

bool Filter::usesEyes() const
{
    return featureUsage[EYES];
}

bool Filter::usesNose() const
{
    return featureUsage[NOSE];
}

bool Filter::usesMouth() const
{
    return featureUsage[MOUTH];
}

void Filter::overlayImage(Mat *src, Mat *overlay, const Point &location) const
{
    for (int y = max(location.y, 0); y < src->rows; ++y)
    {
        int fY = y - location.y;

        if (fY >= overlay->rows)
            break;

        for (int x = max(location.x, 0); x < src->cols; ++x)
        {
            int fX = x - location.x;

            if (fX >= overlay->cols)
                break;

            double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;

            for (int c = 0; opacity > 0 && c < src->channels(); ++c)
            {
                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
            }
        }
    }
}
