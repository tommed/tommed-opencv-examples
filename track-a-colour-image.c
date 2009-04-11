#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>

const int tolerance = 8; // how far away from the colour can be accepted the same colour?

int main(int argc, char *argv[]) {

	if (argc<4) {
		fprintf(stderr, "USAGE: pick-a-colour image_filename red green blue");
		exit(1);
	}
	
	IplImage *src = cvLoadImage(argv[1], 1);
	if (!src) {
		fprintf(stderr, "ERROR: Could not load image, check path.");
		exit(2);
	}
	
	// set the target colour from the command line params
	CvScalar targetColour = CV_RGB((uchar)atoi(argv[2]), (uchar)atoi(argv[3]), (uchar)atoi(argv[4]));
	
	int w,h;
	CvScalar white = CV_RGB(255,255,255);
	CvScalar black = CV_RGB(0,0,0);
	IplImage *clone = cvCloneImage(src);
	fprintf(stdout, "image cloned, processing colour mask..\n");
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

	fprintf(stdout, "mask complete, showing results in mask window\n");
	cvRectangle(clone, cvPoint(minx,miny), cvPoint(maxx,maxy), CV_RGB(255,0,0), 3, 0, 0);
	
	cvNamedWindow("win1", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mask", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("win1", 100, 100);
	cvMoveWindow("win1", 500, 500);
	cvShowImage("win1", src);
	cvShowImage("mask", clone);
	cvWaitKey(0);
	
	cvDestroyWindow("win1");
	cvDestroyWindow("mask");
	cvReleaseImage(&src);
	cvReleaseImage(&clone);
	exit(0);
}
