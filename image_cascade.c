#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main(int argc, char *argv[]) {

	IplImage *src=NULL, *dest=NULL;
	
	fprintf(stdout, "checking arguments\n");
	if (argc<3) {
		fprintf(stderr, "USAGE: image_cascade capture_file cascade_file.\n");
		exit(1);
	}
	
	// load up haartraining
	CvHaarClassifierCascade *cascade = (CvHaarClassifierCascade*)cvLoad(argv[2], 0, 0, 0 );
	if (!cascade) {
		fprintf(stderr, "Could not load cascade, check path");
		exit(2);
	}
	
	fprintf(stdout, "loading image...\n");
	src=cvLoadImage(argv[1], 1);
	fprintf(stdout, "image loaded.\n");
	if (!src) {
		fprintf(stderr, "Could not load your image, please check the filename.\n");
		exit(3);
	}
	
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* faces = NULL;
	dest=cvCloneImage(src);
		
		// facial recognition
		int i=0;
		double t = (double)cvGetTickCount();
		faces = cvHaarDetectObjects(dest, cascade, storage, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(30,30));// SLOWER: 1.1, 2, 0, cvSize(30, 30));
		t = (double)cvGetTickCount() - t;
    printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
		if (!faces || faces->total == 0) {
			fprintf(stdout, "Could not detect a face on this frame\n");
		} else {
			for (i=0; i<faces->total; i++) {
				CvRect* r = (CvRect*)cvGetSeqElem(faces, i);
				cvRectangle(dest, cvPoint(r->x,r->y), cvPoint(r->x + r->width, r->y + r->height), 
					CV_RGB(255,0,0), 1, 1, 0);
			}
		}
		
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
