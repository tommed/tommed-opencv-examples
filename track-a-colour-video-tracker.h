#include <cv.h>
#include <highgui.h>
#include <stdbool.h>

#ifndef TM_VIDEO_CAPTURE_COLOUR_TRACKER
#define TM_VIDEO_CAPTURE_COLOUR_TRACKER 1

// init video capture
bool tmInitVideoCapture(char *filename, uint red, uint green, uint blue, uint tolerance);

// init output window
void tmInitDisplayWindow(bool calibrate);

// get sizes of capture
int tmGetWidth();
int tmGetHeight();

// callback for displays
typedef void (*tmFrameProcessedCallback)(IplImage *img, CvRect *foundAt);

// start processing the frames
void tmProcessFrames(tmFrameProcessedCallback on_frame_processed);

// show the current frame in the window
void tmShowFrame(IplImage *frame);

// dealloc functions
void tmReleaseDisplayWindow();
void tmReleaseVideoCapture();

#endif
