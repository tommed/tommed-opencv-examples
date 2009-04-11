/* http://home.iitk.ac.in/~swagatk/faq/opencv.html#Colar_Based_Tracking */

#include<cv.h>
#include<cxcore.h>
#include<cvaux.h>
#include<highgui.h>

int main(int argc, char *argv[]) {

    CvCapture *capture = cvCaptureFromAVI(argv[1]);
    int frames = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

    uchar *routdata, *goutdata, *target;

    
    //Create a window
    cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Final_RG", CV_WINDOW_AUTOSIZE);    
    cvNamedWindow("Filtered_RG", CV_WINDOW_AUTOSIZE);    


    //Create Image Skeletons
    IplImage *image = 0;
    IplImage *routput = cvCreateImage( cvSize(640,480), 8, 3);
    IplImage *goutput = cvCreateImage( cvSize(640,480), 8, 3);
    IplImage *final_rg = cvCreateImage( cvSize(640,480), 8, 3);
    IplImage *filtered_rg = cvCreateImage( cvSize(640,480), 8, 3);


    int nFrames = 300;
    int j, k, m, n, p;



    // These are used for storing pixel indices 
    int rindex[500][2];
    int gindex[500][2];


    // Threshold for R, G, B channels. This depend on the object to be tracked.
    // These are for a pink object.
    int red = 112;//200;
    int blue = 66;//210;
    int green = 79;//190;

    
    // Video run starts here ...

    for(j = 0; j < nFrames; j++)
    {                 
        image = cvQueryFrame(capture);

        //This is needed for accessing pixel elements
        int step = image->widthStep/sizeof(uchar);

        
        cvCopy(image, routput, NULL);
        cvCopy(image, goutput, NULL);


        //Otherwise the image appears inverted
        routput->origin = image->origin;
        goutput->origin = image->origin;
        final_rg->origin = image->origin;
        filtered_rg->origin = image->origin;



        routdata = (uchar *) routput->imageData;
        goutdata = (uchar *) goutput->imageData;

        k = 0, p = 0;
        for( m = 0; m < image->height; m++) //row
        {
            for( n = 0; n < image->width; n++) //column
            {

                
                // only show those pixels with RG component specified below
                if(routdata[m*step+n*3+2] > red && routdata[m*step+n*3+0] > blue)
                {
                    rindex[k][0] = m;
                    rindex[k][1] = n; // save the pixel indices in a vector for later use.
                    k = k + 1;
                }
                else
                {
                    //black out this pixel
                    routdata[m*step+n*3+0] = 0; //clear blue part
                    routdata[m*step+n*3+1] = 0; //clear green part
                    routdata[m*step+n*3+2] = 0; //clear red part
                }


                // Only show pixels with RB component as specified below
                if(routdata[m*step+n*3+2] > red && routdata[m*step+n*3+1] < blue)
                {
                    gindex[p][0] = m;
                    gindex[p][1] = n; // save the pixel indices in a vector for later use.
                    p = p + 1;       
                }
                else
                {
                    //black out this pixel
                    goutdata[m*step+n*3+0] = 0; //clear blue part
                    goutdata[m*step+n*3+1] = 0; //clear green part
                    goutdata[m*step+n*3+2] = 0; //clear red part
                }

            }
        } 


        // Perform 'AND' Operation between routput and goutput images
        cvAnd( routput, goutput, final_rg, NULL);


        //Apply an averaging filter to remove noise from final_rg image
        cvSmooth(final_rg, filtered_rg, CV_MEDIAN, 5, 5, 0, 0);

        target = (uchar *) filtered_rg->imageData;
        

        int xmin = 400, xmax = 0;
        int ymin = 400, ymax = 0;

        for(m = 0; m < filtered_rg->height; m++)
        {
            for(n = 0; n < filtered_rg->width; n++)
            {

                if(target[m*step+n*3+2] > 100)
                {
                    if(n < xmin)
                        xmin = n;
                    if(n > xmax)
                        xmax = n;
                    if(m < ymin)
                        ymin = m;
                    if(m > ymax)
                        ymax = m;
                }
            }
        }
               
        cvRectangle(image, cvPoint(xmin, ymin), cvPoint(xmax,ymax), CV_RGB(0,255,0),2, 8, 0);
        cvCircle(image, cvPoint((xmin+xmax)/2, (ymin+ymax)/2), 0, CV_RGB(255,0,0),2, 8, 0);
                        



        // Show all the images
        cvShowImage("Image", image);
        cvShowImage( "Final_RG", final_rg);
        cvShowImage ( "Filtered_RG", filtered_rg);       
                    
                
        //wait for a key to press.
        cvWaitKey(0);
    } //for each frame ...


    cvReleaseCapture(&capture);
//    cvReleaseImage(&image);   // it gives error ... find out why?
    cvDestroyWindow("Image");
    cvDestroyWindow("Final_RG");
    cvDestroyWindow("Filtered_RG");

    return 0;

}
