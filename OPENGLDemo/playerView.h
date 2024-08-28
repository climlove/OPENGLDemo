//
//  playerView.h
//  IJKMediaDemo
//
//  Created by Zhimin Zeng on 2024/8/27.
//  Copyright © 2024 bilibili. All rights reserved.
//

#import <GLKit/GLKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface playerView : GLKView

- (BOOL)displayCVPixelBufferRef:(CVPixelBufferRef)videoPicture;
@end

NS_ASSUME_NONNULL_END
