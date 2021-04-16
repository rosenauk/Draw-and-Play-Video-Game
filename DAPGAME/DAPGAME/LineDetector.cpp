//
//  LaneDetector.cpp
//  SimpleLaneDetection
//
//  Created by Anurag Ajwani on 28/04/2019.
//  Copyright © 2019 Anurag Ajwani. All rights reserved.
//

#include "LineDetector.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <algorithm>
using namespace cv;
using namespace std;

using json = nlohmann::json;

//double getAverage(vector<double> vector, int nElements) {
//    
//    double sum = 0;
//    int initialIndex = 0;
//    int last30Lines = int(vector.size()) - nElements;
//    if (last30Lines > 0) {
//        initialIndex = last30Lines;
//    }
//    
//    for (int i=(int)initialIndex; i<vector.size(); i++) {
//        sum += vector[i];
//    }
//    
//    int size;
//    if (vector.size() < nElements) {
//        size = (int)vector.size();
//    } else {
//        size = nElements;
//    }
//    return (double)sum/size;
//}
//
//Mat LaneDetector::detect_lane(Mat image) {
//    
//    Mat colorFilteredImage = filter_only_yellow_white(image);
//    Mat regionOfInterest = crop_region_of_interest(colorFilteredImage);
//    Mat edgesOnly = detect_edges(regionOfInterest);
//    
//    vector<Vec4i> lines;
//    HoughLinesP(edgesOnly, lines, 1, CV_PI/180, 10, 20, 100);
//    
//    return draw_lines(image, lines);
//}


/*
 Parameters:
    type: "x"->cross, "o"->circle, "w"->line
    The geometric center point of the object (x, y)
    arg: The angle between the straight line and the +x axis (0,90), in degrees
    size: the size of game object
 */
void addobj2json(json* gameobjs,String type, double x, double y, double arg, double size){
    json obj;
    obj["type"] = type;
    obj["x"] = x;
    obj["y"] = y;
    obj["angle"] = arg;
    obj["size"] = size;
    
    (*gameobjs)["gameobjs"].push_back(obj);
}

void objs2json(vector<Vec4f> objs, json* gameobjs)
{
    addobj2json(gameobjs,"o",1.0,1.0,90.0,48.0);
    addobj2json(gameobjs,"x",10.0,200.0,90.0,80.0);
    addobj2json(gameobjs,"w",20.0,390.0,0.0,314.0);
    addobj2json(gameobjs,"b",60.0,-240.0,90.0,80.0);
}

String LineDetector::img2json(Mat image) {
    
    cout << "Image is processing" << endl;
    cv::Size size = image.size();
    double height = size.height;
    double width = size.width;
    cout << "Height: "<< height <<endl;
    cout << "Width: "<< width <<endl;
    if (height >720 || width > 720)
    {
        double ratio = 720/max(width,height);
        cout<< "[game]Scale ratio is " << ratio <<endl;
        resize(image, image,cv::Size(),ratio,ratio); //scale to 720*720
        size = image.size();
        height = size.height;
        width = size.width;
    }
    json gameobjects;
    /*
    Mat colorFilteredImage = filter_only_yellow_white(image);
    Mat regionOfInterest = crop_region_of_interest(colorFilteredImage);
    Mat edgesOnly = detect_edges(regionOfInterest);
    
    vector<Vec4i> lines;
    HoughLinesP(edgesOnly, lines, 1, CV_PI/180, 10, 20, 100);
    
    return draw_lines(image, lines);
     */
    
    Mat grayImage,blurImage,cannyImage,output;
    output=image.clone();
    cvtColor(image, grayImage, COLOR_RGB2GRAY);
    medianBlur(grayImage, blurImage, 5);
    vector<Vec3f> circles;
    HoughCircles(blurImage, circles, HOUGH_GRADIENT, 1, image.cols/8, 100, 100, 0, 0);

    for (int i = 0; i < circles.size(); i++)
    {
        Point center((uint16_t)cvRound(circles[i][0]), (uint16_t)cvRound(circles[i][1]));
        double x = (center.x - (width/2))/width * 750 ;
        double y = -1*(center.y - (height/2))/height * 1334 ;
        int radius = (uint16_t)cvRound(circles[i][2])/(width*height/(750*1334));
//        // circle center
        addobj2json(&gameobjects,"o",x,y,90.0,radius);
    }

    
    Canny(grayImage, cannyImage, 50, 200, 3);
    vector<Vec4f> lines;
    HoughLinesP(cannyImage, lines, 1, CV_PI/180, 100, 50, 10);
    for (size_t i = 0; i < lines.size(); i++)
    {
        Point point1 = Point(cvRound(lines[i][0]), cvRound(lines[i][1]));
        Point point2 = Point(cvRound(lines[i][2]), cvRound(lines[i][3]));
        Point center = Point((point1.x+point2.x)/2,(point1.y+point2.y)/2);
        double x = (center.x - (width/2))/width * 750 ;
        double y = -1*(center.y - (height/2))/height * 1334 ;
        double length = cv::norm(point1 - point2)/(width*height/(750*1334));
        double  k = (double)(lines[i][3] - lines[i][1]) / (double)(lines[i][2] - lines[i][0]);
        double angle = atan(k) * 180.0/3.1415926;
        addobj2json(&gameobjects,"w",x,y,angle,length);
    }
    
    //objs2json(lines,&gameobjects);
    //std::cout << gameobjects<< std::endl;
    return gameobjects.dump();

}

Mat LineDetector::detect_line(Mat image) {

    /*
    Mat colorFilteredImage = filter_only_yellow_white(image);
    Mat regionOfInterest = crop_region_of_interest(colorFilteredImage);
    Mat edgesOnly = detect_edges(regionOfInterest);

    vector<Vec4i> lines;
    HoughLinesP(edgesOnly, lines, 1, CV_PI/180, 10, 20, 100);

    return draw_lines(image, lines);
     */
    cv::Size size = image.size();
    double height = size.height;
    double width = size.width;
    
    if (height > 720 || width > 720)
    {
        double ratio = 720/max(width,height);
        cout<< "[game]Scale ratio is " << ratio <<endl;
        resize(image, image,cv::Size(),ratio,ratio); //scale to 720*720
        size = image.size();
        height = size.height;
        width = size.width;
    }
    
    

    Mat grayImage,blurImage,cannyImage,output;
    output=image.clone();
    cvtColor(image, grayImage, COLOR_RGB2GRAY);
    medianBlur(grayImage, blurImage, 5);
    vector<Vec3f> circles;
    HoughCircles(blurImage, circles, HOUGH_GRADIENT, 1, image.cols/8, 100, 100, 0, 0);
//    cout << "Circles" << endl;
//    for (vector<Vec3f>::const_iterator i = circles.begin(); i != circles.end(); ++i)
//        cout << *i << "||";
    for (int i = 0; i < circles.size(); i++)
    {
        Point center((uint16_t)cvRound(circles[i][0]), (uint16_t)cvRound(circles[i][1]));
        int radius = (uint16_t)cvRound(circles[i][2]);
        // circle center
        circle(output, center, 2, Scalar(0, 0, 255), 3);
        // circle outline
        circle(output, center, radius, Scalar(0, 255, 0),2);
    }


    Canny(grayImage, cannyImage, 50, 200, 3);
    vector<Vec4f> lines;
    HoughLinesP(cannyImage, lines, 1, CV_PI/180, 100, 50, 100);
    for (size_t i = 0; i < lines.size(); i++)
    {
        Point point1 = Point(cvRound(lines[i][0]), cvRound(lines[i][1]));
        Point point2 = Point(cvRound(lines[i][2]), cvRound(lines[i][3]));
        line(output, point1, point2, Scalar(255, 0, 0), 3);
    }


    return output;

}

//Mat LineDetector::detect_line(Mat image) {
//    /*
//    Mat colorFilteredImage = filter_only_yellow_white(image);
//    Mat regionOfInterest = crop_region_of_interest(colorFilteredImage);
//    Mat edgesOnly = detect_edges(regionOfInterest);
//
//    vector<Vec4i> lines;
//    HoughLinesP(edgesOnly, lines, 1, CV_PI/180, 10, 20, 100);
//
//    return draw_lines(image, lines);
//     */
//
//    Mat dst, cdst;
//    Canny(image, dst, 50, 200, 3);
//    cvtColor(dst, cdst, COLOR_GRAY2BGR);
//
//    vector<Vec4i> lines;
//    HoughLinesP(dst, lines, 1, CV_PI/180, 10, 20, 100);
//
//    for (size_t i = 0; i < lines.size(); i++)
//        {
//            Vec4i l = lines[i];
//            line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, 4);
//        }
//    return cdst;
//
//}
//
//Mat LaneDetector::filter_only_yellow_white(Mat image) {
//    
//    Mat hlsColorspacedImage;
//    cvtColor(image, hlsColorspacedImage, COLOR_RGB2HLS);
//    
//    Mat yellowMask;
//    Scalar yellowLower = Scalar(10, 0, 90);
//    Scalar yellowUpper = Scalar(50, 255, 255);
//    inRange(hlsColorspacedImage, yellowLower, yellowUpper, yellowMask);
//    
//    Mat whiteMask;
//    Scalar whiteLower = Scalar(0, 190, 0);
//    Scalar whiteUpper = Scalar(255, 255, 255);
//    inRange(hlsColorspacedImage, whiteLower, whiteUpper, whiteMask);
//    
//    Mat mask;
//    bitwise_or(yellowMask, whiteMask, mask);
//    
//    Mat maskedImage;
//    bitwise_and(image, image, maskedImage, mask);
//    
//    return maskedImage;
//}
//
//Mat LaneDetector::crop_region_of_interest(Mat image) {
//    
//    /*
//     The code below draws the region of interest into a new image of the same dimensions as the original image.
//     The region of interest is filled with the color we want to filter for in the image.
//     Lastly it combines the two images.
//     The result is only the color within the region of interest.
//     */
//    
//    int maxX = image.rows;
//    int maxY = image.cols;
//    
//    Point shape[1][5];
//    shape[0][0] = Point(0, maxX);
//    shape[0][1] = Point(maxY, maxX);
//    shape[0][2] = Point((int)(0.55 * maxY), (int)(0.6 * maxX));
//    shape[0][3] = Point((int)(0.45 * maxY), (int)(0.6 * maxX));
//    shape[0][4] = Point(0, maxX);
//    
//    Scalar color_to_filter(255, 255, 255);
//    
//    Mat filledPolygon = Mat::zeros(image.rows, image.cols, CV_8UC3); // empty image with same dimensions as original
//    const Point* polygonPoints[1] = { shape[0] };
//    int numberOfPoints[] = { 5 };
//    int numberOfPolygons = 1;
//    fillPoly(filledPolygon, polygonPoints, numberOfPoints, numberOfPolygons, color_to_filter);
//    
//    // Cobine images into one
//    Mat maskedImage;
//    bitwise_and(image, filledPolygon, maskedImage);
//    
//    return maskedImage;
//}
//
//Mat LaneDetector::draw_lines(Mat image, vector<Vec4i> lines) {
//    
//    vector<double> rightSlope, leftSlope, rightIntercept, leftIntercept;
//    
//    for (int i=0; i<lines.size(); i++) {
//        Vec4i line = lines[i];
//        double x1 = line[0];
//        double y1 = line[1];
//        double x2 = line[2];
//        double y2 = line[3];
//        
//        double yDiff = y1-y2;
//        double xDiff = x1-x2;
//        double slope = yDiff/xDiff;
//        double yIntecept = y2 - (slope*x2);
//        
//        if ((slope > 0.3) && (x1 > 500)) {
//            rightSlope.push_back(slope);
//            rightIntercept.push_back(yIntecept);
//        } else if ((slope < -0.3) && (x1 < 600)) {
//            leftSlope.push_back(slope);
//            leftIntercept.push_back(yIntecept);
//        }
//    }
//    
//    double leftAvgSlope = getAverage(leftSlope, 30);
//    double leftAvgIntercept = getAverage(leftIntercept, 30);
//    double rightAvgSlope = getAverage(rightSlope, 30);
//    double rightAvgIntercept = getAverage(rightIntercept, 30);
//    
//    int leftLineX1 = int(((0.65*image.rows) - leftAvgIntercept)/leftAvgSlope);
//    int leftLineX2 = int((image.rows - leftAvgIntercept)/leftAvgSlope);
//    int rightLineX1 = int(((0.65*image.rows) - rightAvgIntercept)/rightAvgSlope);
//    int rightLineX2 = int((image.rows - rightAvgIntercept)/rightAvgSlope);
//    
//    Point shape[1][4];
//    shape[0][0] = Point(leftLineX1, int(0.65*image.rows));
//    shape[0][1] = Point(leftLineX2, int(image.rows));
//    shape[0][2] = Point(rightLineX2, int(image.rows));
//    shape[0][3] = Point(rightLineX1, int(0.65*image.rows));
//    
//    const Point* polygonPoints[1] = { shape[0] };
//    int numberOfPoints[] = { 4 };
//    int numberOfPolygons = 1;
//    Scalar fillColor(0, 0, 255);
//    fillPoly(image, polygonPoints, numberOfPoints, numberOfPolygons, fillColor);
//    
//    Scalar rightColor(0,255,0);
//    Scalar leftColor(255,0,0);
//    line(image, shape[0][0], shape[0][1], leftColor, 10);
//    line(image, shape[0][3], shape[0][2], rightColor, 10);
//    
//    return image;
//}
//
//Mat LaneDetector::detect_edges(Mat image) {
//    
//    Mat greyScaledImage;
//    cvtColor(image, greyScaledImage, COLOR_RGB2GRAY);
//    
//    Mat edgedOnlyImage;
//    Canny(greyScaledImage, edgedOnlyImage, 50, 120);
//    
//    return edgedOnlyImage;
//}
