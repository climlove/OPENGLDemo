//
//  HTYMenuVC.m
//  HTY360Player
//
//  Created by  on 11/8/15.
//  Copyright © 2015 Hanton. All rights reserved.
//

#import "HTYMenuVC.h"
#import <MobileCoreServices/MobileCoreServices.h>
#import "HTY360PlayerVC.h"
#import "IJKMoviePlayerViewController.h"

@interface HTYMenuVC () <UIImagePickerControllerDelegate, UINavigationControllerDelegate>

@end

@implementation HTYMenuVC

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        
    }
    return self;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskLandscape;
}

#pragma mark - Button Action

- (IBAction)playDemo:(UIButton *)sender {
    [self launchVideoWithName:@"360panorama"];
}

- (IBAction)playijk:(UIButton *)sender {
    NSString *path = [[NSBundle mainBundle] pathForResource:@"360panorama" ofType:@"mp4"];
    NSURL *url = [[NSURL alloc] initFileURLWithPath:path];
    [self presentViewController:[[IJKVideoViewController alloc] initWithURL:url] animated:YES completion:nil];
}



#pragma mark - Private Method

- (void)launchVideoWithName:(NSString*)name {
    NSString *path = [[NSBundle mainBundle] pathForResource:name ofType:@"mp4"];
    NSURL *url = [[NSURL alloc] initFileURLWithPath:path];
    HTY360PlayerVC *videoController = [[HTY360PlayerVC alloc] initWithNibName:@"HTY360PlayerVC"
                                                                       bundle:nil
                                                                          url:url];
    
    if (![[self presentedViewController] isBeingDismissed]) {
        [self presentViewController:videoController animated:YES completion:nil];
    }
}

- (void)openURLWithString:(NSString*)stringURL {
    NSString *path = [[NSBundle mainBundle] pathForResource:@"360panorama" ofType:@"mp4"];
    NSURL *url = [[NSURL alloc] initFileURLWithPath:path];
    [self.navigationController pushViewController:[[IJKVideoViewController alloc] initWithURL:url] animated:YES];
}

#pragma mark - UIImagePickerControllerDelegate

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    [picker dismissViewControllerAnimated:YES completion:nil];
    
    NSURL *url = [info objectForKey:UIImagePickerControllerMediaURL];
    HTY360PlayerVC *videoController = [[HTY360PlayerVC alloc] initWithNibName:@"HTY360PlayerVC"
                                                                       bundle:nil
                                                                          url:url];
    
    [self presentViewController:videoController animated:YES completion:nil];
}

@end
