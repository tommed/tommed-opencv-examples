#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main(int argc, char *argv[]) {

	IplImage *src = NULL;
	IplImage *dest = NULL;
	int height,width,step,channels;
	uchar *data;
	int i,j,k;
	
	fprintf(stdout, "checking arguments\n");
	if (argc<2) {
		fprintf(stderr, "You need to supply a filename as an argument.\n");
		exit(1);
	}
	
	fprintf(stdout, "loading image...\n");
	src=cvLoadImage(argv[1], 1);
	fprintf(stdout, "image loaded.\n");
	if (!src) {
		fprintf(stderr, "Could not load your image, please check the filename.\n");
		exit(2);
	}
	
	// get dimensions
	height = src->height;
	width = src->width;
	step = src->widthStep;
	channels = src->nChannels;
	data = (uchar *)src->imageData;
	printf("Processing a %dx%d image with %d channels.\n", width,height,channels);
	
	fprintf(stdout, "Image Loaded.\n");
	
	// uncomment *one* of the following lines to create dest image
	//dest=cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3); // 3=BGR, 1=GREYSCALE
	//dest=cvCloneImage(src);
	
	// flip or switch R/B channels (use with cvCreateImage)
	//cvConvertImage(src, dest, CV_CVTIMG_FLIP); //CV_CVTIMG_SWAP_RB);
	
	// greyscale (use with cvCreateImage)
	//cvCvtColor(src, dest, CV_BGR2GRAY);
	
	/*
	// draw a box and circle (use with cvCloneImage)
	cvRectangle(dest, cvPoint(350,50), cvPoint(600,400), CV_RGB(255,0,0), 3, CV_AA, 0); // 3=thickness, CV_AA=lintype-antialias // 0=shift
	cvCircle(dest, cvPoint(450,150), 100, CV_RGB(0,255,0), 3, CV_AA, 0);
	cvLine(dest, cvPoint(350,50), cvPoint(600,400), CV_RGB(0,0,255), 3, CV_AA, 0);
	*/
	
	/*
	// add text (use with cvCloneImage)
	CvFont font;
	double hScale=0.8, vScale=0.8; // font size
	int lineWidth=2;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX,hScale,vScale,0,lineWidth,CV_AA); // shear, thickness, lineType
	cvPutText(dest, "Hello World!", cvPoint(350,400), &font, CV_RGB(0,0,0));
	*/
	
	/*
	// edge detection (use with cvCreateImage & cvCvtColor as both input and output images HAVE to be grayscale)
	//cvCanny(dest, dest, 80, 80, 3);
	////IplImage *cornerHarrisImage = cvCreateImage(cvGetSize(dest), IPL_DEPTH_32F, 1);
	////cvCornerHarris(dest, cornerHarrisImage, 3, 7, 2);
	*/
	
	
	cvNamedWindow("imageWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("imageWin", 100, 100);
	cvShowImage("imageWin", dest);
	cvWaitKey(0); // wait for key press

	cvDestroyWindow("imageWin");	
	cvReleaseImage(&src);
	cvReleaseImage(&dest);
	//cvReleaseImage(&cornerHarrisImage);
	exit(0);
}
