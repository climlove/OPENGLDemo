//
//  displayView.h
//  IJKMediaDemo
//
//  Created by Zhimin Zeng on 2024/8/27.
//  Copyright © 2024 bilibili. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <IJKMediaPlayerKit/IJKMediaPlayerKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol displayViewDelegate <NSObject>

- (void)playCVPixelBufferRef:(CVPixelBufferRef)BufferRef;

@end

@interface displayView : UIView<IJKVideoRenderingProtocol>
@property (nonatomic,weak)id<displayViewDelegate> delegate;
@end

NS_ASSUME_NONNULL_END
