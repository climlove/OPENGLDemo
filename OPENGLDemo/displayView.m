//
//  displayView.m
//  IJKMediaDemo
//
//  Created by Zhimin Zeng on 2024/8/27.
//  Copyright © 2024 bilibili. All rights reserved.
//

#import "displayView.h"
#import "PanoramaRenderer.h"
@interface displayView()
{
    BOOL isFirst;
}
@end
@implementation displayView

- (BOOL)displayAttach:(IJKOverlayAttach *)attach{
//你需要的当前画面：attach.videoPicture
    CVPixelBufferRef videoPicture = attach.videoPicture;
    if([self.delegate respondsToSelector:@selector(playCVPixelBufferRef:)])
    {
        [self.delegate playCVPixelBufferRef:videoPicture];
    }
    return YES;
}

- (id)context
{
    return nil;
}

- (void)setScalingMode:(IJKMPMovieScalingMode)scalingMode
{

}
- (NSString *)name
{
    return @"display";
}






/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
