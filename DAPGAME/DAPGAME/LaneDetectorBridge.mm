//
//  LangeDetectorBridge.m
//  SimpleLaneDetection
//


#import <opencv2/opencv.hpp>
#import <opencv2/imgcodecs/ios.h>
#import <Foundation/Foundation.h>
#import "LineDetectorBridge.h"
#include "LineDetector.hpp"
#include <string>

@implementation LineDetectorBridge
    

- (UIImage *) detectLine: (UIImage *) image {
    
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

- (NSString *) image2map: (UIImage *) image {
    
    cv::Mat opencvImage;
    UIImageToMat(image, opencvImage, true);
    
    // convert colorspace to the one expected by the lane detector algorithm (RGB)
    cv::Mat convertedColorSpaceImage;
    cv::cvtColor(opencvImage, convertedColorSpaceImage, COLOR_RGBA2RGB);
    
    // Run lane detection
    LineDetector lineDetector;
    std::string stringresult= lineDetector.img2json(convertedColorSpaceImage);
    NSString *nstringresult = [NSString stringWithCString:stringresult.c_str()
                                       encoding:[NSString defaultCStringEncoding]];
    
    return nstringresult;
}
    
    @end
