//
//  LangeDetectorBridge.m
//  SimpleLaneDetection
//
//  Created by Anurag Ajwani on 28/04/2019.
//  Copyright © 2019 Anurag Ajwani. All rights reserved.
//

#import <opencv2/opencv.hpp>
#import <opencv2/imgcodecs/ios.h>
#import <Foundation/Foundation.h>
#import "LineDetectorBridge.h"
#include "LineDetector.hpp"


@implementation LineDetectorBridge
    
//- (UIImage *) detectLaneIn: (UIImage *) image {
//    
//    // convert uiimage to mat
//    cv::Mat opencvImage;
//    UIImageToMat(image, opencvImage, true);
//    
//    // convert colorspace to the one expected by the lane detector algorithm (RGB)
//    cv::Mat convertedColorSpaceImage;
//    cv::cvtColor(opencvImage, convertedColorSpaceImage, COLOR_RGBA2RGB);
//    
//    // Run lane detection
//    LaneDetector laneDetector;
//    cv::Mat imageWithLaneDetected = laneDetector.detect_lane(convertedColorSpaceImage);
//    
//    // convert mat to uiimage and return it to the caller
//    return MatToUIImage(imageWithLaneDetected);
//}

- (UIImage *) detectLineIn: (UIImage *) image {
    
    // convert uiimage to mat
    cv::Mat opencvImage;
    UIImageToMat(image, opencvImage, true);
    
    // convert colorspace to the one expected by the lane detector algorithm (RGB)
    cv::Mat convertedColorSpaceImage;
    cv::cvtColor(opencvImage, convertedColorSpaceImage, COLOR_RGBA2RGB);
    
    // Run lane detection
    LineDetector lineDetector;
    cv::Mat imageWithLaneDetected = lineDetector.detect_line(convertedColorSpaceImage);
    
    // convert mat to uiimage and return it to the caller
    return MatToUIImage(imageWithLaneDetected);
}

- (NSMutableArray *) image2map: (UIImage *) image {
    static NSMutableArray *array = [NSMutableArray arrayWithCapacity:2];
    

    return array;
}
    
    @end
