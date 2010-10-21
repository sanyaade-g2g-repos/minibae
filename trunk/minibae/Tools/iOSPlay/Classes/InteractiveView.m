//
//  InteractiveView.m
//  iOSPlay
//
//  Created by Steve Hales on 10/9/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//

#import "InteractiveView.h"
#import <CoreGraphics/CGGeometry.h>
#import <UIKit/UIStringDrawing.h>

@implementation InteractiveView

typedef struct
{
	int			mMidiNote;
	NSString*	mMidiNoteName;
} KeyNotes;

#define OCTAVE(x) ((x)*7)
#define NOTE_C(x) 0+OCTAVE(x)
#define NOTE_D(x) 1+OCTAVE(x)
#define NOTE_E(x) 2+OCTAVE(x)
#define NOTE_F(x) 3+OCTAVE(x)
#define NOTE_G(x) 4+OCTAVE(x)
#define NOTE_A(x) 6+OCTAVE(x)
#define NOTE_B(x) 5+OCTAVE(x)

static KeyNotes sKeyList[] =
{
	{-1, @"C0"},	// octave 0
	{-1, @"D0"},	// 
	{-1, @"E0"},	// 
	{-1, @"F0"},	// 
	{-1, @"G0"},	// 
	{21, @"A0"},	// 
	{23, @"B0"},	// 
	{24, @"C1"},	// octave 1 
	{26, @"D1"},	// 
	{28, @"E1"},	// 
	{29, @"F1"},	// 
	{31, @"G1"},	// 
	{33, @"A1"},	// 
	{35, @"B1"},	// 
	{36, @"C2"},	// octave 2
	{38, @"D2"},	// 
	{40, @"E2"},	// 
	{41, @"F2"},	// 
	{43, @"G2"},	// 
	{45, @"A2"},	// 
	{47, @"B2"},	// 
	{48, @"C3"},	// octave 3
	{50, @"D3"},	// 
	{52, @"E3"},	// 
	{53, @"F3"},	// 
	{55, @"G3"},	// 
	{57, @"A3"},	// 
	{59, @"B3"},	// 
	{60, @"C4"},	// octave 4
	{62, @"D4"},	// 
	{64, @"E4"},	// 
	{65, @"F4"},	// 
	{67, @"G4"},	// 
	{69, @"A4"},	// 
	{71, @"B4"},	// 
	{72, @"C5"},	// octave 5
	{74, @"D5"},	// 
	{76, @"E5"},	// 
	{77, @"F5"},	// 
	{79, @"G5"},	// 
	{81, @"A5"},	// 
	{83, @"B5"},	// 
	{84, @"C6"},	// octave 6
	{86, @"D6"},	// 
	{88, @"E6"},	// 
	{89, @"F6"},	// 
	{91, @"G6"},	// 
	{93, @"A6"},	// 
	{95, @"B6"},	// 
	{96, @"C7"},	// octave 7
	{98, @"D7"},	// 
	{100, @"E7"},	// 
	{101, @"F7"},	// 
	{103, @"G7"},	// 
	{105, @"A7"},	// 
	{107, @"B7"},	// 
	{108, @"C8"},	// octave 8
	{ -1, @"D8"},	// 
	{ -1, @"E8"},	// 
	{ -1, @"F8"},	// 
	{ -1, @"G8"},	// 
	{ -1, @"A8"},	// 
	{ -1, @"B8"},	// 
};

typedef struct
{
	CGRect		mBound;
	int			mKey;
	NSString*	mMidiNoteName;
	BOOL		mPressed;
} KeySquare;

#define KEY_SQUARE_MAX	(6*7)
static KeySquare sSquares[KEY_SQUARE_MAX];

static int sKeyUsed[KEY_SQUARE_MAX] = 
{
	NOTE_E(3), NOTE_C(3), NOTE_A(2), NOTE_D(3), NOTE_G(2), NOTE_F(2),
	NOTE_E(5), NOTE_C(5), NOTE_A(4), NOTE_D(5), NOTE_G(4), NOTE_F(4),
	NOTE_E(4), NOTE_C(4), NOTE_A(3), NOTE_D(4), NOTE_G(3), NOTE_F(3),
	NOTE_D(7), NOTE_B(6), NOTE_C(5), NOTE_A(5), NOTE_D(5), NOTE_G(4),
	NOTE_D(6), NOTE_B(5), NOTE_C(4), NOTE_A(4), NOTE_D(4), NOTE_G(3),
	NOTE_E(7), NOTE_C(7), NOTE_G(5), NOTE_B(5), NOTE_F(5), NOTE_A(4),
	NOTE_E(6), NOTE_C(6), NOTE_G(4), NOTE_B(4), NOTE_F(4), NOTE_A(3),
};

#define KEY_LEFT_EDGE 20
#define KEY_RIGHT_EDGE -20
#define KEY_TOP_EDGE 20
#define KEY_BOTTOM_EDGE -135

// http://anythingpeaceful.org/sonar/gm/gm-instruments.html
typedef struct
{
	unsigned char	mInstrument;
	unsigned char	mChannel;
} KeySquareSetup;

#define PIANO		{0, 0}
#define STRING		{103, 1}
#define LEAD_SYNTH	{80, 2}
static KeySquareSetup sKeySetup[KEY_SQUARE_MAX] =
{
	PIANO, PIANO, PIANO, PIANO, PIANO, PIANO, 
	LEAD_SYNTH, LEAD_SYNTH, LEAD_SYNTH, LEAD_SYNTH, LEAD_SYNTH, LEAD_SYNTH, 
	STRING, STRING, STRING, STRING, STRING, STRING, 
	STRING, STRING, STRING, STRING, STRING, STRING, 
	STRING, STRING, STRING, STRING, STRING, STRING, 
	STRING, STRING, STRING, STRING, STRING, STRING, 
	STRING, STRING, STRING, STRING, STRING, STRING, 
};

- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		[self setMultipleTouchEnabled:YES];

		CGRect area = frame;
		area.origin.x += KEY_LEFT_EDGE;
		area.origin.y += KEY_TOP_EDGE;
		area.size.width += (KEY_RIGHT_EDGE - KEY_LEFT_EDGE);
		area.size.height += (KEY_BOTTOM_EDGE - KEY_TOP_EDGE);

		square.origin.x = area.origin.x;
		square.origin.y = area.origin.y;
		square.size.height = (area.size.height / 7);
		square.size.width = (area.size.width / 6);
		for (int count = 0; count < KEY_SQUARE_MAX; count++)
		{
			sSquares[count].mKey = sKeyList[sKeyUsed[count]].mMidiNote;
			sSquares[count].mMidiNoteName = sKeyList[sKeyUsed[count]].mMidiNoteName;

			sSquares[count].mBound = square;
			square.origin.x += square.size.width;
			if (square.origin.x >= area.size.width)
			{
				square.origin.x = area.origin.x;
				square.origin.y += square.size.height;
			}
			sSquares[count].mPressed = FALSE;
		}

		iOSPlayAppDelegate* app = (iOSPlayAppDelegate*)[[UIApplication sharedApplication] delegate];
		mInteractiveSong = BAESong_New([app getMixer]);
		
		for (int count = 0; count < KEY_SQUARE_MAX; count++)
		{
			unsigned char instrument = sKeySetup[count].mInstrument;
			unsigned char channel = sKeySetup[count].mChannel;
			printf("Load sq %d inst %d chan %d\n", count, instrument, channel);
			BAEResult result = BAESong_LoadInstrument(mInteractiveSong, instrument);
			if (result)
			{
				printf("Bad load %d\n", instrument);
			}
			BAESong_ProgramChange(mInteractiveSong, channel, instrument, 0);
		}
    }
    return self;
}

- (void)removeFromSuperview
{
	[super removeFromSuperview];
	BAESong_Delete(mInteractiveSong);
	mInteractiveSong = NULL;
	printf("free InteractiveView\n");
}


void drawLinearGradient(CGContextRef context, CGRect rect, CGColorRef startColor, 
						CGColorRef  endColor) 
{
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGFloat locations[] = { 0.0, 1.0 };
	
	NSArray* colors = [NSArray arrayWithObjects:(id)startColor, (id)endColor, (id)nil];
	
    CGGradientRef gradient = CGGradientCreateWithColors(colorSpace, 
														(CFArrayRef) colors, locations);
	
	CGPoint startPoint = CGPointMake(CGRectGetMidX(rect), CGRectGetMinY(rect));
	CGPoint endPoint = CGPointMake(CGRectGetMidX(rect), CGRectGetMaxY(rect));
	
	CGContextSaveGState(context);
	CGContextAddRect(context, rect);
	CGContextClip(context);
	CGContextDrawLinearGradient(context, gradient, startPoint, endPoint, 0);
	CGContextRestoreGState(context);
	
	CGGradientRelease(gradient);
	CGColorSpaceRelease(colorSpace);
}


const float ROT_90 = (M_PI * 90.0 / 180.0);
const float ROT_270 = (M_PI * 270.0 / 180.0);


// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect 
{
	//Get the CGContext from this view
	CGContextRef context = UIGraphicsGetCurrentContext();

	CGColorRef whiteColor = [UIColor colorWithRed:1.0 green:1.0 
											 blue:1.0 alpha:1.0].CGColor; 
	CGColorRef lightGrayColor = [UIColor colorWithRed:230.0/255.0 green:230.0/255.0 
												 blue:230.0/255.0 alpha:1.0].CGColor;
	// draw a nice paper background
	CGRect paperRect = self.bounds;
	drawLinearGradient(context, paperRect, whiteColor, lightGrayColor);

	// fill squares
	for (int count = 0; count < KEY_SQUARE_MAX; count++)
	{
		if (sSquares[count].mPressed)
		{
			CGContextSetRGBFillColor(context, 0.6, 0.3, 1.0, 1.0);
			CGContextFillRect(context, sSquares[count].mBound);
		}
	}

	// Draw borders
	CGContextSetRGBStrokeColor(context, 0.3, 0.6, 1.0, 1.0);	
	// Draw them with a 2.0 stroke width so they are a bit more visible.
	CGContextSetLineWidth(context, 2.0);
	
	for (int count = 0; count < KEY_SQUARE_MAX; count++)
	{
		CGContextAddRect(context, sSquares[count].mBound);
	}
	CGContextStrokePath(context);

	// draw text labels
    CGContextSelectFont(context, "Helvetica-Bold", 24.0, kCGEncodingMacRoman);
    CGContextSetTextDrawingMode(context, kCGTextFill);

    CGAffineTransform xform = CGAffineTransformMakeRotation(ROT_270);
	xform = CGAffineTransformScale(xform, -1.0, 1.0);
	xform = CGAffineTransformTranslate(xform, 0, 0);
    CGContextSetTextMatrix(context, xform);
	
	for (int count = 0; count < KEY_SQUARE_MAX; count++)
	{
		const char* noteName = [sSquares[count].mMidiNoteName cStringUsingEncoding:NSMacOSRomanStringEncoding];
		NSString* noteNumberName = [[NSString alloc] initWithFormat: @"%d", sSquares[count].mKey];
		const char* noteNumber = [noteNumberName cStringUsingEncoding:NSMacOSRomanStringEncoding];

		CGPoint p = sSquares[count].mBound.origin;
		if (sSquares[count].mPressed)
		{
			CGContextSetRGBFillColor(context, 1.0, 1.0, 1.0, 1.0);
		}
		else
		{
			CGContextSetRGBFillColor(context, 0.0, 0.0, 0.0, 1.0);
		}
		CGContextShowTextAtPoint(context, p.x + (square.size.width * .8), p.y + 5, noteName, strlen(noteName));
		CGContextShowTextAtPoint(context, p.x + (square.size.width * .3), p.y + 5, noteNumber, strlen(noteNumber));
	}

	// draw outer border
	CGContextSetRGBStrokeColor(context, 1.0, 0, 0, 1.0);
	CGContextAddRect(context, self.bounds);
	CGContextStrokePath(context);
}

- (void)dealloc 
{
    [super dealloc];
}

//
// multi touch proxies
//
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	CGPoint point;
	
	int idx = 0;
	for (UITouch *touch in touches)
	{
		point = [touch locationInView: [touch view]];
		idx++;
		//printf("touch begin %f %f\n", point.x, point.y);
		for (int count = 0; count < KEY_SQUARE_MAX; count++)
		{
			if (CGRectContainsPoint(sSquares[count].mBound, point))
			{
				sSquares[count].mPressed = TRUE;
				BAESong_NoteOn(mInteractiveSong, sKeySetup[count].mChannel, sSquares[count].mKey, 100, 0);
				[self setNeedsDisplayInRect:sSquares[count].mBound];
				printf("note on %d\n", sSquares[count].mKey);
			}
		}
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	int idx = 0;
	for (UITouch *touch in touches)
	{
		CGPoint point = [touch locationInView: [touch view]];
		CGPoint prevPoint = [touch previousLocationInView: [touch view]];

		idx++;
		//printf("touch moved %f %f\n", point.x, point.y);
		for (int count = 0; count < KEY_SQUARE_MAX; count++)
		{
			BOOL newPress = CGRectContainsPoint(sSquares[count].mBound, point);
			BOOL oldPress = CGRectContainsPoint(sSquares[count].mBound, prevPoint);

			if (oldPress != newPress)
			{
				if (newPress)
				{
					if (sSquares[count].mPressed == FALSE)
					{
						sSquares[count].mPressed = TRUE;
						BAESong_NoteOn(mInteractiveSong, sKeySetup[count].mChannel, sSquares[count].mKey, 100, 0);
						printf("note on %d\n", sSquares[count].mKey);
					}
				}
				if (oldPress)
				{
					if (sSquares[count].mPressed)
					{
						sSquares[count].mPressed = FALSE;
						BAESong_NoteOff(mInteractiveSong, sKeySetup[count].mChannel, sSquares[count].mKey, 100, 0);
						printf("note off %d\n", sSquares[count].mKey);
					}
				}
				[self setNeedsDisplayInRect:sSquares[count].mBound];
			}
		}
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	BOOL allTouchesEnded = ([touches count] == [[event touchesForView:self] count]);
	
	int idx = 0;
	for (UITouch *touch in touches)
	{
		CGPoint point = [touch locationInView: [touch view]];
		//CGPoint prevPoint = [touch previousLocationInView: [touch view]];
		
		idx++;
		//printf("touch end %f %f\n", point.x, point.y);
		for (int count = 0; count < KEY_SQUARE_MAX; count++)
		{
			if (sSquares[count].mPressed)
			{
				if (CGRectContainsPoint(sSquares[count].mBound, point))
				{
					[self setNeedsDisplayInRect:sSquares[count].mBound];
					sSquares[count].mPressed = FALSE;
					BAESong_NoteOff(mInteractiveSong, sKeySetup[count].mChannel, sSquares[count].mKey, 100, 0);
					printf("note off %d\n", sSquares[count].mKey);
				}
			}
		}
	}
	//if (allTouchesEnded)
	{
		for (int count = 0; count < KEY_SQUARE_MAX; count++)
		{
			if (sSquares[count].mPressed)
			{
				printf("note %d still on\n", sSquares[count].mKey);
			}
		}
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{	
	int idx = 0;
	for (UITouch *touch in touches)
	{
		CGPoint point = [touch locationInView: [touch view]];
		CGPoint prevPoint = [touch previousLocationInView: [touch view]];

		idx++;
		printf("touch cancel %f %f\n", point.x, point.y);
		for (int count = 0; count < KEY_SQUARE_MAX; count++)
		{
			if (CGRectContainsPoint(sSquares[count].mBound, prevPoint))
			{
				sSquares[count].mPressed = FALSE;
				[self setNeedsDisplayInRect:sSquares[count].mBound];
			}
		}
	}
}



@end
