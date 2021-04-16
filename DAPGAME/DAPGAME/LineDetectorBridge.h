//
//  LaneDetectorBridge.h
//  SimpleLaneDetection
//


#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface LineDetectorBridge : NSObject
    
- (UIImage *) detectLine: (UIImage *) image;
- (NSString *) image2map: (UIImage *) image;
@end
