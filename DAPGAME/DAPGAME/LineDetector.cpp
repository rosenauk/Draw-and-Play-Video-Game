//
//  LaneDetector.cpp
//  SimpleLaneDetection
//

#include "LineDetector.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <algorithm>
using namespace cv;
using namespace std;

using json = nlohmann::json;

#define GAP 5 //Set the pixel pitch
const int screen_height = 1334 * 0.8;
const int screen_width = 750 * 0.8;


bool cmp1(const Vec4f &a, const Vec4f &b) {
    return a[1] < b[1];//x1,y1,x2,y2,
}
bool cmp2(const Vec4f &a, const Vec4f &b) {
    return a[0] < b[0];//x1,y1,x2,y2
}

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

//void objs2json(vector<Vec4f> objs, json* gameobjs) // a demo, used for testing
//{
//    addobj2json(gameobjs,"o",1.0,1.0,90.0,48.0);
//    addobj2json(gameobjs,"x",10.0,200.0,90.0,80.0);
//    addobj2json(gameobjs,"w",20.0,390.0,0.0,314.0);
//    addobj2json(gameobjs,"b",60.0,-240.0,90.0,80.0);
//}

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
        double x = (center.x - (width/2))/width * screen_width ;
        double y = -1*(center.y - (height/2))/height * screen_height;
        int radius = (uint16_t)cvRound(circles[i][2])/(width*height/(screen_width*screen_height));
//        // circle center
        addobj2json(&gameobjects,"o",x,y,90.0,radius);
    }

    
    Canny(grayImage, cannyImage, 50, 200, 3);
    vector<Vec4f> lines;
    HoughLinesP(cannyImage, lines, 1, CV_PI/180, 100, 75, 30);
    
    
/* Filter lines begin*/
    std::vector<cv::Vec4f> lines_horizon, lines_vertical,lines_slanted;
        for (int i = 0; i < lines.size(); ++i) {
            cv::Vec4i line_ = lines[i];
            double k = (double)(line_[3] - line_[1]) / (double)(line_[2] - line_[0]);//The slope of the line
            double angle = atan(k) * 180 / CV_PI;//Straight angle
            //horizontal direction 0°/180°/-180°/360°
            if ((angle >= -1 && angle <= 1) || (angle >= 359 && angle <= 361) || (angle >= 179 && angle <= 181) || (angle >= -181 && angle <= -179)) {//horizontal lines
                lines_horizon.push_back(line_);
            }//vertical direction 90°/-90°/270°/-270°
            else if ((angle >= 89 && angle <= 91) || (angle >= -91 && angle <= -89) || (angle >= 269 && angle <= 271) || (angle >= -271 && angle <= -269)) {//vertical lines
                lines_vertical.push_back(line_);
            }
            else lines_slanted.push_back(line_);
        }
    
    
/* detect cross*/
    
    if(lines_slanted.size()>=2){

        Point2f p1 = Point2f(cvRound(lines_slanted[0][0]), cvRound(lines_slanted[0][1]));
        Point2f p2 = Point2f(cvRound(lines_slanted[0][2]), cvRound(lines_slanted[0][3]));
        for (size_t i = 1; i < lines_slanted.size(); i++)
        {
            Point2f p3 = Point2f(cvRound(lines_slanted[i][0]), cvRound(lines_slanted[i][1]));
            Point2f p4 = Point2f(cvRound(lines_slanted[i][2]), cvRound(lines_slanted[i][3]));
            Point2f x = p3 - p1;
            Point2f d1 = p2 - p1;
            Point2f d2 = p4 - p3;
            float cross = d1.x*d2.y - d1.y*d2.x;
            if (abs(cross) >= /*EPS*/1e-8) {
                double t1 = (x.x * d2.y - x.y * d2.x)/cross;
                Point2f center =  p1 + d1 * t1;
                double xx = (center.x - (width/2))/width * screen_width ;
                double yy = -1*(center.y - (height/2))/height * screen_height ;
                addobj2json(&gameobjects,"x",xx,yy,0.0,80.0); //add cross to gameobjects
                break; // only keep the first cross
            }

        }
    }
    /* detect cross end*/
    
    
        sort(lines_horizon.begin(), lines_horizon.end(), cmp1);//水平方向按照y升排
        sort(lines_vertical.begin(), lines_vertical.end(), cmp2);//垂直方向按照x升排
     
        std::vector<cv::Vec4f> filter_horizon, filter_vertical, filter_lines;
        int streak_gap = 1 * GAP;
     
    //Filter horizontal lines
        if (lines_horizon.size() > 0) {
            filter_horizon.push_back(lines_horizon[0]);
            for (int i = 1; i < lines_horizon.size(); ++i) {
                int gap = lines_horizon[i][1] - lines_horizon[i - 1][1];
                if (gap > streak_gap) {
                    filter_horizon.push_back(lines_horizon[i]);//keep the first line If they are very close on the x-axis.
                }
            }
        }
        //Filter verticle lines
        if (lines_vertical.size() > 0) {
            filter_vertical.push_back(lines_vertical[0]);
            for (int i = 1; i < lines_vertical.size(); ++i) {
                int gap = lines_vertical[i][0] - lines_vertical[i - 1][0];
                if (gap > streak_gap) {
                    filter_vertical.push_back(lines_vertical[i]); //keep the first line If they are very close on the y-axis.
                }
            }
        }

//    int res = filter_horizon.size() + filter_vertical.size();
//    cout<<"There are "<<res<<" lines"<<endl;
    filter_lines.reserve( filter_horizon.size() + filter_vertical.size() ); // preallocate memory
    filter_lines.insert( filter_lines.end(), filter_horizon.begin(), filter_horizon.end() );
    filter_lines.insert( filter_lines.end(), filter_vertical.begin(), filter_vertical.end() );
    /* Filter lines end*/
    
    
    for (size_t i = 0; i < filter_lines.size(); i++)
    {
        Point point1 = Point(cvRound(filter_lines[i][0]), cvRound(filter_lines[i][1]));
        Point point2 = Point(cvRound(filter_lines[i][2]), cvRound(filter_lines[i][3]));
        Point center = Point((point1.x+point2.x)/2,(point1.y+point2.y)/2);
        double x = (center.x - (width/2))/width * screen_width ;
        double y = -1*(center.y - (height/2))/height * screen_height ;
        double length = cv::norm(point1 - point2)/(width*height/(screen_width*screen_height));
        double  k = (double)(filter_lines[i][3] -filter_lines[i][1]) / (double)(filter_lines[i][2] -filter_lines[i][0]);
        double angle = atan(k) * 180.0/CV_PI;
        addobj2json(&gameobjects,"w",x,y,angle,length); //add lines to game
    }
    
    return gameobjects.dump();

}

/* !!! LineDetector::detect_line and LineDetector::img2json are actully doing the same thing, But they are inconsistent now, this function needs to be updated !!! */

Mat LineDetector::detect_line(Mat image) {


    cv::Size size = image.size();
    double height = size.height;
    double width = size.width;
    
    if (height > 720 || width > 720)
    {
        double ratio = 720/max(width,height);
        cout<< "[Preview]Scale ratio is " << ratio <<endl;
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
    HoughLinesP(cannyImage, lines, 1, CV_PI/180, 100, 75, 30);
    for (size_t i = 0; i < lines.size(); i++)
    {
        Point point1 = Point(cvRound(lines[i][0]), cvRound(lines[i][1]));
        Point point2 = Point(cvRound(lines[i][2]), cvRound(lines[i][3]));
        line(output, point1, point2, Scalar(255, 0, 0), 3);
    }


    return output;

}

