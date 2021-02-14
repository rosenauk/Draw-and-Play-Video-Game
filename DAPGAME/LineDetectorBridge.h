//
//  LineDetectorBridge.h
//  Draw and Play Video Game
//
//  Created by Matthew Alonso on 1/27/21.
//

#ifndef LineDetectorBridge_h
#define LineDetectorBridge_h


#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface LineDetectorBridge : NSObject

- (UIImage *) detectLineIn: (UIImage *) image;

@end

#endif /* LineDetectorBridge_h */
