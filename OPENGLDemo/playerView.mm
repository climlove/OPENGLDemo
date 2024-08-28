//
//  playerView.m
//  IJKMediaDemo
//
//  Created by Zhimin Zeng on 2024/8/27.
//  Copyright © 2024 bilibili. All rights reserved.
//

#import "playerView.h"
#import "PanoramaRenderer.h"
@interface playerView()
{
    PanoramaRenderer renderer;
    EAGLContext *context;
}
@end
@implementation playerView
- (instancetype)init
{
    self = [super init];
    if (self) {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        [EAGLContext setCurrentContext:context];
        self.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
        renderer = PanoramaRenderer();
        renderer.onSurfaceCreated();
    }
    return self;
}
- (BOOL)displayCVPixelBufferRef:(CVPixelBufferRef)videoPicture{
//你需要的当前画面：attach.videoPicture

    if(videoPicture!= NULL){
        renderer.processUI([self cvMatFromUIImage:[self uiImageFromPixelBuffer:videoPicture]]);
        renderer.onDrawFrame();
    }
    return YES;
}
-(UIImage*)uiImageFromPixelBuffer:(CVPixelBufferRef)pixelBufferRef {

    CVPixelBufferRef p = CVPixelBufferRetain(pixelBufferRef);
    if(p== nil)
    return nil;
    CVPixelBufferLockBaseAddress(p, 0);
    CIImage* ciImage = [CIImage imageWithCVPixelBuffer : p];

    CIContext* context = [CIContext contextWithOptions : @{kCIContextUseSoftwareRenderer : @(YES)}];

    CGRect rect = CGRectMake(0, 0, CVPixelBufferGetWidth(p), CVPixelBufferGetHeight(p));

    CGImageRef videoImage = [context createCGImage : ciImage fromRect : rect];
    CVPixelBufferUnlockBaseAddress(p, 0);

    UIImage* image = [UIImage imageWithCGImage : videoImage];

    CGImageRelease(videoImage);

    return image;

}
   

- (cv::Mat)cvMatFromUIImage:(UIImage *)image
{
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(image.CGImage);
    CGFloat cols = image.size.width;
    CGFloat rows = image.size.height;
    cv::Mat cvMat(rows, cols, CV_8UC4); // 8 bits per component, 4 channels (color channels + alpha)
    CGContextRef contextRef = CGBitmapContextCreate(cvMat.data,                 // Pointer to  data
                                                    cols,                       // Width of bitmap
                                                    rows,                       // Height of bitmap
                                                    8,                          // Bits per component
                                                    cvMat.step[0],              // Bytes per row
                                                    colorSpace,                 // Colorspace
                                                    kCGImageAlphaNoneSkipLast |
                                                    kCGBitmapByteOrderDefault); // Bitmap info flags
    CGContextDrawImage(contextRef, CGRectMake(0, 0, cols, rows), image.CGImage);
    CGContextRelease(contextRef);
    cv::cvtColor(cvMat, cvMat, cv::COLOR_BGR2RGB);
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *path = [paths objectAtIndex:0];
    path = [NSString stringWithFormat:@"%@/img.png",path];
  //  cv::imwrite(path.UTF8String, cvMat);
    return cvMat;
}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
