/*
 * CIS 190: C++ Programming - Project
 * http://www.cis.upenn.edu/~cis190/
 * Last Modified: 04 Dec 2019
 * Ayush Parikh (ayushp@seas.upenn.edu)
 * Dennis Gann (dgann@seas.upenn.edu)
 */

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <list>
#include <vector>

#include "debug_filter.hpp"
#include "flower_filter.hpp"
#include "dog_filter.hpp"
#include "dapper_filter.hpp"
#include "image_algorithms.hpp"

#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define WINDOW_NAME "Selection Pane"

using namespace std;
using namespace cv;

Mat processInputImage(Mat inputImage, Filter *currFilter);
Mat processSelectedMode(Mat inputImage, int mode, Filter *currFilter);
int drawModeButtons(Mat frame, int mode);
void drawSaveButton(Mat frame, Mat inputImage);

CascadeClassifier classifiers[4];
bool somethingSaved = false;
int photoCount = 1;

enum Mode
{
    NORMAL,
    INVERTED,
    CANNY,
    HISTOGRAM_BW,
    HISTOGRAM_RGB,
    FILTER_DAPPER,
    FILTER_DOG,
    FILTER_FLOWER,
    FILTER_DEBUG
};

int main(int argc, const char **argv)
{
    Filter *currFilter = nullptr;

    // parse the command line args
    CommandLineParser parser(argc, argv,
                             "{help h||}"
                             "{debug d||}"
                             "{cascadePath cp|data/haarcascades/|Path to the haar cascades.}"
                             "{camera c|0|Camera device number.}");
    parser.about("\nPhoto Booth App.\n\n");
    parser.printMessage();
    string const cascadePath = parser.get<string>("cascadePath");
    bool const debug = parser.get<bool>("debug");
    int const cameraDevice = parser.get<int>("camera");

    // load the cascade classifiers
    if (!classifiers[FACE].load(cascadePath + "Face.xml") ||
        !classifiers[EYES].load(cascadePath + "EyePair.xml") ||
        !classifiers[MOUTH].load(cascadePath + "Mouth.xml") ||
        !classifiers[NOSE].load(cascadePath + "Nose.xml"))
    {
        cout << "(!) Error: loading haar cascades.\n";
        return -1;
    };

    // create the filters
    DebugFilter debugFilter{"Debug"};
    FlowerFilter flowerFilter{"Flower"};
    DogFilter dogFilter{"Dog"};
    DapperFilter dapperFilter{"Dapper"};

    // get the video stream
    VideoCapture capture;
    capture.open(cameraDevice);
    if (!capture.isOpened())
    {
        cout << "(!) Error: opening video stream.\n";
        return -1;
    }

    // Create a frame to render UI components
    Mat frame = cv::Mat(350, 550, CV_8UC3);

    // Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
    cvui::init(WINDOW_NAME);

    //Modes for selection
    int mode = NORMAL;
    double contrastValue = 1.0;
    double brightnessValue = 0.0;

    if (debug)
    {
        currFilter = &debugFilter;
        mode = FILTER_DEBUG;
    }

    // main event loop
    Mat inputImage;
    while (capture.isOpened())
    {
        capture.read(inputImage);

        if (inputImage.empty())
        {
            cout << "(!) Error: No captured inputImage.\n";
            break;
        }

        frame = cv::Scalar(0x0, 0x0, 0x80);
        mode = drawModeButtons(frame, mode);

        if (mode == FILTER_DAPPER)
        {
            currFilter = &dapperFilter;
        }
        else if (mode == FILTER_DOG)
        {
            currFilter = &dogFilter;
        }
        else if (mode == FILTER_FLOWER)
        {
            currFilter = &flowerFilter;
        }

        inputImage = processSelectedMode(inputImage, mode, currFilter);

        cvui::text(frame, 110, 10, "CONTRAST", 0.5, 0x80);
        cvui::trackbar(frame, 40, 30, 220, &contrastValue, (double)0, (double)5);

        cvui::text(frame, 375, 10, "BRIGHTNESS", 0.5, 0x80);
        cvui::trackbar(frame, 310, 30, 220, &brightnessValue, (double)-100, (double)100);

        inputImage.convertTo(inputImage, -1, contrastValue, brightnessValue);

        drawSaveButton(frame, inputImage);

        cv::namedWindow("Photo Booth", WINDOW_AUTOSIZE);
        cvui::imshow("Photo Booth", inputImage);
        cvui::imshow(WINDOW_NAME, frame);

        // if ESC is pressed then exit
        if (waitKey(10) == 27)
        {
            break;
        }
    }

    return 0;
}

Mat processInputImage(Mat inputImage, Filter *currFilter)
{
    // Detect faces
    vector<Rect> faces;
    classifiers[FACE].detectMultiScale(inputImage, faces, 1.1, 3, 0, Size(100, 100));
    // loop through each detected face
    for (auto face : faces)
    {
        if (currFilter && currFilter->usesFace())
        {
            currFilter->applyFace(inputImage, face);
        }

        Mat faceRegion = inputImage(face);
        bool eyeCoordsSet = false;
        Point eyeCoordTopLeft;
        Point eyeCoordBottomRight;

        // check if eyes needed
        if (currFilter && currFilter->usesEyes())
        {
            // In each face, detect eyes
            std::vector<Rect> eyePairs;
            // only consider half the face for eyes
            Mat topFace = faceRegion(Rect(0, 0, faceRegion.cols, faceRegion.rows / 2));
            classifiers[EYES].detectMultiScale(topFace, eyePairs);

            // loop through each pair of eyes
            for (auto eyePair : eyePairs)
            {
                eyeCoordTopLeft = Point(eyePair.x, eyePair.y);
                eyeCoordBottomRight = Point(eyePair.x + eyePair.width, eyePair.y + eyePair.height);
                eyeCoordsSet = true;

                currFilter->applyEyes(inputImage, face, eyePair);
                break; // exist after first eye pair, since should only be one anyway
            }
        }

        if (currFilter && currFilter->usesNose())
        {
            std::vector<Rect> noses;
            classifiers[NOSE].detectMultiScale(faceRegion, noses, 1.1, 10);
            for (auto nose : noses)
            {
                // check to make sure nose is in middle of face
                if (eyeCoordsSet && !(nose.x < eyeCoordTopLeft.x || nose.y < eyeCoordTopLeft.y || (nose.x + nose.width) > eyeCoordBottomRight.x))
                {
                    currFilter->applyNose(inputImage, face, nose);
                    break;
                }
            }
        }

        if (currFilter && currFilter->usesMouth())
        {
            std::vector<Rect> mouths;
            // only consider bottom half the face for eyes
            Mat bottomFace = faceRegion(Rect(0, faceRegion.rows / 2, faceRegion.cols, faceRegion.rows / 2));
            classifiers[MOUTH].detectMultiScale(bottomFace, mouths, 1.3, 10);
            for (auto mouth : mouths)
            {
                currFilter->applyMouth(inputImage, face, mouth);
                break;
            }
        }
    }

    return inputImage;
}

Mat processSelectedMode(Mat inputImage, int mode, Filter *currFilter)
{
    if (mode == INVERTED)
    {
        return ImageAlgorithms::inverted(inputImage);
    }
    else if (mode == CANNY)
    {
        return ImageAlgorithms::cannyEdge(inputImage);
    }
    else if (mode == HISTOGRAM_BW)
    {
        return ImageAlgorithms::histogramBW(inputImage);
    }
    else if (mode == HISTOGRAM_RGB)
    {
        return ImageAlgorithms::histogramRGB(inputImage);
    }
    else if (mode >= FILTER_DAPPER && mode <= FILTER_DEBUG)
    {
        return processInputImage(inputImage, currFilter);
    }
    else
    {
        return inputImage;
    }
}

int drawModeButtons(Mat frame, int mode)
{
    if (cvui::button(frame, 10, 90, 100, 30, "Original"))
    {
        mode = NORMAL;
    }

    if (cvui::button(frame, 10, 140, 100, 30, "Inverted"))
    {
        mode = INVERTED;
    }

    if (cvui::button(frame, 150, 90, 100, 30, "Canny Edge"))
    {
        mode = CANNY;
    }

    if (cvui::button(frame, 290, 90, 100, 30, "BW Histogram"))
    {
        mode = HISTOGRAM_BW;
    }

    if (cvui::button(frame, 430, 90, 100, 30, "RGB Histogram"))
    {
        mode = HISTOGRAM_RGB;
    }

    if (cvui::button(frame, 150, 140, 100, 30, "Dapper Filter"))
    {
        mode = FILTER_DAPPER;
    }

    if (cvui::button(frame, 290, 140, 100, 30, "Dog Filter"))
    {
        mode = FILTER_DOG;
    }

    if (cvui::button(frame, 430, 140, 100, 30, "Flower Filter"))
    {
        mode = FILTER_FLOWER;
    }

    return mode;
}

void drawSaveButton(Mat frame, Mat inputImage)
{
    if (cvui::button(frame, 225, 200, 100, 30, "Save Image"))
    {
        std::string filename("snapshot" + std::to_string(photoCount) + ".jpg");
        cv::imwrite(filename, inputImage);
        somethingSaved = true;
        photoCount++;
    }

    if (somethingSaved)
    {
        cvui::text(frame, 120, 250, "snapshot" + std::to_string(photoCount - 1) + ".jpg" + " saved!", 1, 0x80);
    }

    cvui::text(frame, 140, 300, "Press Esc to Quit", 1, 0x80);
}