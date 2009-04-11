#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>

CvScalar targetColour;
int red, green, blue;

void changeRed(int position) {
	red = position;
	targetColour.val[2] = (uchar)red;
}

void changeGreen(int position) {
	green = position;
	targetColour.val[1] = (uchar)green;
}

void changeBlue(int position) {
	blue = position;
	targetColour.val[0] = (uchar)blue;
}


int main(int argc, char *argv[]) {

	if (argc<4) {
		fprintf(stderr, "USAGE: pick-a-colour video_filename red green blue tolerance\n");
		exit(1);
	}
	
	fprintf(stdout, "Loading file: %s\n", argv[1]);
	CvCapture *capture = cvCaptureFromAVI(argv[1]);
	if (!capture) {
		fprintf(stderr, "ERROR: Could not load video, check path.\n");
		exit(2);
	}
	
	// set the target colour from the command line params
	red=atoi(argv[2]); green=atoi(argv[3]); blue=atoi(argv[4]);
	targetColour = CV_RGB((uchar)red, (uchar)green, (uchar)blue);
	int tolerance = atoi(argv[5]); // how far away from the colour can be accepted the same colour?

	// create output windows
	IplImage *frame = NULL;
	cvNamedWindow("win1", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("mask", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("red", "win1", &red, 255, changeRed);
	cvCreateTrackbar("green", "win1", &green, 255, changeGreen);
	cvCreateTrackbar("blue", "win1", &blue, 255, changeBlue);
	cvCreateTrackbar("colour tolerance", "win1", &tolerance, 50, NULL);
		
	int totalCount=0, matchCount=0;
	for (;;) {
		frame = cvQueryFrame(capture);
		if (!frame) break;
		
		int w,h;
		CvScalar white = CV_RGB(255,255,255);
		CvScalar black = CV_RGB(0,0,0);
		IplImage *clone = cvCloneImage(frame);
		//fprintf(stdout, "image cloned, processing colour mask..\n");
		int minx=10000, miny=10000;
		int maxx=0, maxy=0;
		for (h=0; h<clone->height; h++) for (w=0; w<clone->width; w++) {
			CvScalar currentColour = cvGet2D(clone,h,w);
			if (currentColour.val[0] >= targetColour.val[0]-tolerance && // compare BGR
					currentColour.val[0] <= targetColour.val[0]+tolerance &&
					currentColour.val[1] >= targetColour.val[1]-tolerance &&
					currentColour.val[1] <= targetColour.val[1]+tolerance &&
					currentColour.val[2] >= targetColour.val[2]-tolerance &&
					currentColour.val[3] <= targetColour.val[2]+tolerance ) {
						// if they are the same then make this cell white
						cvSet2D(clone,h,w,white);
						if (w>maxx) maxx=w; if (h>maxy) maxy=h;
						if (w<minx) minx=w; if (h<miny) miny=h;
						//fprintf(stdout, "colour match found at %dx%d\n", w, h);
			} else {
						// if they are NOT the same then make this cell black
						cvSet2D(clone,h,w,black);
			}
		}

		//fprintf(stdout, "mask complete, showing results in mask window\n");
		totalCount++;
		if (minx < 10000 && miny < 10000) {
			matchCount++;
			//fprintf(stdout, "found item of that colour in frame!\n");
			cvRectangle(frame, cvPoint(minx,miny), cvPoint(maxx,maxy), CV_RGB(255,0,0), 3, 0, 0);
		}
	
		cvShowImage("win1", frame);
		//cvShowImage("mask", clone);
		cvWaitKey(3);
		cvReleaseImage(&clone);
	}
	
	// display tracker 
	fprintf(stdout, "Match rating %d/%d (%.2f percent)\n", matchCount, totalCount, 
				  ((double)matchCount / (double)totalCount)*100);
	
	cvWaitKey(0);
	cvDestroyWindow("win1");
	//cvDestroyWindow("mask");
	exit(0);
}
