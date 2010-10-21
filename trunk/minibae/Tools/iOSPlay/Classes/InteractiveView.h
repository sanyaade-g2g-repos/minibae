//
//  InteractiveView.h
//  iOSPlay
//
//  Created by Steve Hales on 10/9/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "iOSPlayAppDelegate.h"


@interface InteractiveView : UIView 
{
	CGRect square;
	BAESong	mInteractiveSong;
}

void drawLinearGradient(CGContextRef context, CGRect rect, CGColorRef startColor, 
						CGColorRef  endColor);

@end
