#include <cv.h>
#include <highgui.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "track-a-colour-video-tracker.h"

// calibration settings
uint TmColourRed;
uint TmColourGreen;
uint TmColourBlue;
uint TmColourTolerance;
CvCapture *TmCapture = NULL;
CvScalar TmTargetColour;

//
// Prototype the calibration callback functions
//
void on_red_calibrated(int position);
void on_green_calibrated(int position);
void on_blue_calibrated(int position);

//
// Initialize the Video Capture structs and pass in initial colour calibration settings
//
bool tmInitVideoCapture(char *filename, uint red, uint green, uint blue, uint tolerance) {
	TmColourRed = red;
	TmColourGreen = green;
	TmColourBlue = blue;
	TmColourTolerance = tolerance;
	TmCapture = (CvCapture *)cvCaptureFromAVI(filename);
	return TmCapture != NULL;
}

//
// Create the display window which will show the video.
// if @calibrate is set to TRUE then show calibration controls.
//
void tmInitDisplayWindow(bool calibrate) {
	cvNamedWindow("tmWin1", CV_WINDOW_AUTOSIZE);
	if (calibrate) {
		cvCreateTrackbar("Red", "tmWin1", &TmColourRed, 255, on_red_calibrated);
		cvCreateTrackbar("Green", "tmWin1", &TmColourGreen, 255, on_green_calibrated);
		cvCreateTrackbar("Blue", "tmWin1", &TmColourBlue, 255, on_blue_calibrated);
		cvCreateTrackbar("Colour Tolerance", "tmWin1", &TmColourTolerance, 50, NULL);
	}
}

void tmProcessFrames(tmFrameProcessedCallback on_frame_processed) {
	//fprintf(stdout, "-inside tmProcessFrames\n");
	IplImage *frame = NULL;
	TmTargetColour = CV_RGB((uchar)TmColourRed, (uchar)TmColourGreen, (uchar)TmColourBlue);
	for (;;) {
		//fprintf(stdout, "-capture\n");
		frame = cvQueryFrame(TmCapture);
		if (!frame) break;
		int w,h;
		int minx=10000, miny=10000;
		int maxx=0, maxy=0;
		for (h=0; h<frame->height; h++) for (w=0; w<frame->width; w++) {
			CvScalar currentColour = cvGet2D(frame,h,w);
			if (currentColour.val[0] >= TmTargetColour.val[0]-TmColourTolerance && // compare BGR
					currentColour.val[0] <= TmTargetColour.val[0]+TmColourTolerance &&
					currentColour.val[1] >= TmTargetColour.val[1]-TmColourTolerance &&
					currentColour.val[1] <= TmTargetColour.val[1]+TmColourTolerance &&
					currentColour.val[2] >= TmTargetColour.val[2]-TmColourTolerance &&
					currentColour.val[3] <= TmTargetColour.val[2]+TmColourTolerance ) {
						// if they are the same then make this cell white
						if (w>maxx) maxx=w; if (h>maxy) maxy=h;
						if (w<minx) minx=w; if (h<miny) miny=h;
			}
		}

		//fprintf(stdout, "-building rect\n");
		CvRect *foundAt = NULL;
		if (minx < 10000 && miny < 10000) {
			CvRect _fa = cvRect(minx, miny, maxx-minx, maxy-miny);
			foundAt = &_fa;
		}
	
		//fprintf(stdout, "-callback\n");
		if (on_frame_processed) {
			on_frame_processed(frame, foundAt);
		}
	}
}

//
// Call this to show the given frame in the window.
//
void tmShowFrame(IplImage *frame) {
	cvShowImage("tmWin1", frame);
	cvWaitKey(3);
}

//
// Release the display window.
//
void tmReleaseDisplayWindow() {
	cvDestroyWindow("tmWin1");
}

//
// Release the video capture
//
void tmReleaseVideoCapture() {
	if (TmCapture) 
		cvReleaseCapture(&TmCapture);
}

//
// Events for calibration controls.
//

void on_red_calibrated(int position) {
	TmTargetColour.val[2] = position;
}

void on_green_calibrated(int position) {
	TmTargetColour.val[1] = position;
}

void on_blue_calibrated(int position) {
	TmTargetColour.val[0] = position;
}
