//
//  LineDetectorBridge.m
//  Draw and Play Video Game
//
//  Created by Matthew Alonso on 1/27/21.
//
#import <opencv2/opencv.hpp>
#import <opencv2/imgcodecs/ios.h>
#import <Foundation/Foundation.h>
#import "LineDetectorBridge.h"
#include "LineDetector.hpp"

#import <Foundation/Foundation.h>

@implementation LineDetectorBridge

- (UIImage *) detectLineIn: (UIImage *) image {
    
    // convert uiimage to mat
    cv::Mat opencvImage;
    UIImageToMat(image, opencvImage, true);
    
    // convert colorspace to the one expected by the lane detector algorithm (RGB)
    cv::Mat convertedColorSpaceImage;
    cv::cvtColor(opencvImage, convertedColorSpaceImage, CV_RGBA2RGB);
    
    // Run lane detection
    LineDetector lineDetector;
    cv::Mat imageWithLaneDetected = lineDetector.detect_line(convertedColorSpaceImage);
    
    // convert mat to uiimage and return it to the caller
    return MatToUIImage(imageWithLaneDetected);
}

@end
