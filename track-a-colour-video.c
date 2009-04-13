#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <cv.h>
#include <highgui.h>
#include <X11/Xlib.h>
#include "track-a-colour-video-tracker.h"

void on_frame(IplImage *frame, CvRect *foundAt);

CvFont font;

int main(int argc, char *argv[]) {
	fprintf(stdout, "main()\n");
	bool success;
	char *filename = argv[1];
	uint red = (uint)atoi(argv[2]);
	uint green = (uint)atoi(argv[3]);
	uint blue = (uint)atoi(argv[4]);
	uint tolerance = (uint)atoi(argv[5]);
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX,0.6,0.6,0,1,0);//CV_AA); // shear, thickness, lineType
	
	fprintf(stdout, "tmInitVideoCapture\n");
	success = tmInitVideoCapture(filename, red, green, blue, tolerance);
	if (!success) {
		fprintf(stderr, "Could not start video capture, check path.");
		exit(1);
	}

	fprintf(stdout, "tmInitDisplayWindow\n");
	tmInitDisplayWindow(false);
	
	fprintf(stdout, "tmProcessFrames\n");
	tmProcessFrames(on_frame);
	cvWaitKey(0);

	fprintf(stdout, "tmDealloc\n");
	tmReleaseDisplayWindow();
	tmReleaseVideoCapture();
	exit(0);
}

void on_frame(IplImage *frame, CvRect *foundAt) {
	//fprintf(stdout, "on_frame\n");
	if (foundAt != NULL) 
	{
		// write co-ordinates at top of screen
		char *output = (char *)malloc(sizeof(char)*30);
		sprintf(output, "X=%d, Y=%d, W=%d, H=%d", foundAt->x, foundAt->y, foundAt->width, foundAt->height);
		cvPutText(frame, output, cvPoint(10,40), &font, CV_RGB(0,0,0));
		free(output);
	
		// draw rectangle around object
		cvRectangle(frame, cvPoint(foundAt->x, foundAt->y), 
											 cvPoint(foundAt->x+foundAt->width, foundAt->y+foundAt->height), 
											 CV_RGB(255,0,0), 3, 0, 0);	
	}
	tmShowFrame(frame);
}
