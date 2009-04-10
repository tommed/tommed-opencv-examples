#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>

#define CASCADE_FILE "/usr/share/opencv/haarcascades/haarcascade_profileface.xml" /*frontalface_alt.xml"*/

int main(int argc, char *argv[]) {

	if (argc<2) {
		fprintf(stderr, "You must specify the path to a video file\n");
		exit(1);
	}
	
	CvCapture *capture=cvCaptureFromAVI(argv[1]);
	if (!capture) {
		fprintf(stderr, "Could not load avi, please check path.");
		exit(2);
	}
	
	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("src", 100, 100);
	
	cvNamedWindow("dest", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("dest", 600, 100);
	
	// facial recognition
	CvHaarClassifierCascade *cascade = (CvHaarClassifierCascade*)cvLoad(CASCADE_FILE, 0, 0, 0 );
	if (!cascade) {
		fprintf(stderr, "Could not load cascade, check path");
		exit(3);
	}
	
	IplImage *frameImage = NULL;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* faces = NULL;
	int i;
	for(;;) {
		frameImage=cvQueryFrame(capture);
		if (NULL == frameImage) break; // capture all frames
		IplImage *dup = cvCloneImage(frameImage);
		
		// facial recognition
		double t = (double)cvGetTickCount();
		faces = cvHaarDetectObjects(dup, cascade, storage, 1.3, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(30,30));// SLOWER: 1.1, 2, 0, cvSize(30, 30));
		t = (double)cvGetTickCount() - t;
    printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
		if (!faces || faces->total == 0) {
			fprintf(stdout, "Could not detect a face on this frame\n");
		} else {
			for (i=0; i<faces->total; i++) {
				CvRect* r = (CvRect*)cvGetSeqElem(faces, i);
				cvRectangle(dup, cvPoint(r->x,r->y), cvPoint(r->x + r->width, r->y + r->height), 
					CV_RGB(255,0,0), 1, 1, 0);
			}
		}
		
		cvShowImage("src", frameImage);
		cvShowImage("dest", dup);
		cvWaitKey(5);
		//cvReleaseImage(&frameImage); // NOTE: docs say: DO NOT release this image!
		cvReleaseImage(&dup);
	}
	
	fprintf(stdout, "Done.");
	cvWaitKey(500); // 500ms wait after movie is complete
	cvReleaseCapture(&capture);
	cvDestroyWindow("dest");
	cvDestroyWindow("src");
	exit(0);
}
