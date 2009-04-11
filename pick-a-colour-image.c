#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>

const int tolerance = 8; // how far away from the colour can be accepted the same colour?
IplImage *clone = NULL;
CvScalar *targetColour = NULL;


CvScalar getPixelColour(const IplImage *img, const int x, const int y) {
	uchar blue  = ((uchar *)(img->imageData + x*img->widthStep))[y*img->nChannels + 0];
	uchar green = ((uchar *)(img->imageData + x*img->widthStep))[y*img->nChannels + 1];
	uchar red   = ((uchar *)(img->imageData + x*img->widthStep))[y*img->nChannels + 2];
	return CV_RGB(red, green, blue);
}

void onMouseClick(int event, int x, int y, int flags, void *param) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		fprintf(stdout, "left button clicked\n");
		IplImage *currentFrame = (IplImage *)param;
		fprintf(stdout, "Image found with dimensions %dx%d\n", currentFrame->width, currentFrame->height);
		*targetColour = getPixelColour(currentFrame,x,y);
		fprintf(stdout, "Color at %dx%d is: RGB(%.0f,%.0f,%.0f)\n", x, y, targetColour->val[2], targetColour->val[1], targetColour->val[0]);
		
		int w,h;
		CvScalar white = CV_RGB(255,255,255);
		CvScalar black = CV_RGB(0,0,0);
		IplImage *clone = cvCloneImage(currentFrame);
		fprintf(stdout, "image cloned, processing colour mask..\n");
		int minx=10000, miny=10000;
		int maxx=0, maxy=0;
		for (h=0; h<clone->height; h++) for (w=0; w<clone->width; w++) {
			CvScalar currentColour = cvGet2D(clone,h,w);
			if (currentColour.val[0] >= targetColour->val[0]-tolerance && // compare BGR
					currentColour.val[0] <= targetColour->val[0]+tolerance &&
				  currentColour.val[1] >= targetColour->val[1]-tolerance &&
					currentColour.val[1] <= targetColour->val[1]+tolerance &&
					currentColour.val[2] >= targetColour->val[2]-tolerance &&
					currentColour.val[3] <= targetColour->val[2]+tolerance ) {
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
		//cvSmooth(clone, clone, CV_GAUSSIAN, 3, 0, 0, 0); // apply a blur so if a few pixels get missed out we can still account for them
		cvRectangle(clone, cvPoint(minx,miny), cvPoint(maxx,maxy), CV_RGB(255,0,0), 3, 0, 0);
		cvShowImage("mask", clone);
	}
}

int main(int argc, char *argv[]) {

	if (argc<2) {
		fprintf(stderr, "USAGE: pick-a-colour image_filename");
		exit(1);
	}
	
	IplImage *src = cvLoadImage(argv[1], 1);
	if (!src) {
		fprintf(stderr, "ERROR: Could not load image, check path.");
		exit(2);
	}
	
	cvNamedWindow("win1", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mask", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("win1", 100, 100);
	cvMoveWindow("win1", 500, 500);
	cvSetMouseCallback("win1", onMouseClick, src);
	cvShowImage("win1", src);
	cvShowImage("mask", src);
	cvWaitKey(0);
	
	cvDestroyWindow("win1");
	cvDestroyWindow("mask");
	cvReleaseImage(&src);
	if (clone) cvReleaseImage(&clone);
	exit(0);
}
