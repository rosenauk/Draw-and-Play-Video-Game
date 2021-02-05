//
//  OpencvWrap.m
//  Draw and Play Video Game
//
//  Created by user183596 on 1/21/21.
//

#import "OpencvWrap.h"

@implementation OpencvWrap

+ (NSString *)openCVVersionString {
return [NSString stringWithFormat:@"OpenCV Version %s",  CV_VERSION];
}
@end
