#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

const char* image_label_on_off	= "patrones/onoff.png";
const char* image_led_on 	= "patrones/led_on.png";
const char* image_led_off 	= "patrones/led_off.png";

#define DEBUG

#define kAA_OFF		0
#define kAA_ON		1
#define kAA_ERROR	2

/// Function Headers
void MatchingMethod( int, void* );
void search(Mat&,Mat&,Point&);
int isOn(Mat&, Mat&, Mat&, Mat&);


/** @function main */
int main( int argc, char** argv )
{
  Mat source, led_on, led_off, label_on_off;
  Mat resized;
  Point location_on_off;

  // Load image and patterns
  led_on 	= imread(image_led_on, CV_LOAD_IMAGE_COLOR);
  led_off 	= imread(image_led_off, CV_LOAD_IMAGE_COLOR);
  label_on_off 	= imread(image_label_on_off, CV_LOAD_IMAGE_COLOR);

  if(argc!=2)
  {
     cout <<"Run with...\n";
     cout <<"p5 filename\n";
     return(1);
  }
  source = imread( argv[1], CV_LOAD_IMAGE_COLOR);

  // Check if all images are ok
  if(source.data==NULL || led_on.data==NULL || led_off.data==NULL || label_on_off.data==NULL)
     return(2);

  if(source.cols!=640 || source.rows!=360)
  {
     Size size(640,360);
     resize(source,resized,size);
     cout << isOn(resized,label_on_off,led_on,led_off);
  }
  else
  {
     cout << isOn(source,label_on_off,led_on,led_off);
  }
  return 0;
}




int isOn(Mat &original, Mat &label_on_off, Mat &led_on, Mat &led_off)
{
    Point loc1,loc2,loc3;
    bool on=false, off=false;

    search(original, label_on_off, loc1);
    //cout <<"LOCATION LABEL: ("<<loc1.x<<","<<loc1.y<<")\n\n\n\n";

    //Mat searchZoneOnOff(original, cv::Rect(loc1.x+label_on_off.cols, loc1.y-led_on.rows*2, led_on.cols*3, led_on.rows*3));
    Rect zonaOnOff(loc1.x+label_on_off.cols, loc1.y-led_on.rows*2, led_on.cols*3, led_on.rows*3);
    //cout <<"Search Zone: ("<<loc1.x+label_on_off.cols<<","<<loc1.y-led_on.rows*2<<") -> ("<<(loc1.x+label_on_off.cols)+led_on.cols*3<<","<<(loc1.y-led_on.rows*2)+led_on.rows*3<<")\n";

    //Buscamos los dos patrones del led, y el que caiga dentro de la zonaOnOff, es el correcto
    search(original,led_on,loc2);
    //cout <<" -> LOCATION LED_ON: ("<<loc2.x<<","<<loc2.y<<")\n\n\n";

    search(original,led_off,loc3);
    //cout <<"LOCATION LED_OFF: ("<<loc3.x<<","<<loc3.y<<")\n\n\n";

    if(zonaOnOff.contains(loc2))
       on=true;
    if(zonaOnOff.contains(loc3))
       off=true;

#ifdef DEBUG
    if(on && !off)
       cout << "Result: ON\n";
    else if(!on && off)
       cout << "Result: OFF\n";
    else
       cout << "Result: ERROR\n";
#endif

    if(on && !off)
       return(kAA_ON);
    else if(!on && off)
       return(kAA_OFF);
    return(kAA_ERROR);
}



void search( Mat &original, Mat &patron, Point& location)
{
  double minVal, maxVal;
  Point minLoc,maxLoc;
  Mat resultado;

  /// Create the result matrix
  int result_cols =  original.cols - patron.cols + 1;
  int result_rows = original.rows - patron.rows + 1;

  resultado.create( result_rows, result_cols, CV_32FC1 );

  /// Do the Matching and Normalize
  matchTemplate( original, patron, resultado, CV_TM_SQDIFF );
  normalize( resultado, resultado, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  minMaxLoc( resultado, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

  location.x=minLoc.x;
  location.y=minLoc.y;
}
