#include <stdio.h>      /* printf */
#include <math.h> 

#include "ColorMaps.h"

using std::cout;
using std::endl;

namespace scrimmage {

Color_t GetColor_matlab(double v, double vmin, double vmax)
{
     Color_t c = {255.0,255.0,255.0}; // white
     double dv;

     if (v < vmin) {
          v = vmin;
     } else if (v > vmax) {
          v = vmax;
     }
     
     dv = vmax - vmin;

     double mx = v*0.5*vmax / (0.125*vmax - vmin);
     
     if (v < (vmin + 0.125 * dv)) {
          c.r = vmin;
          c.g = vmin;
          c.b = mx + 0.5*vmax;
     } else if (v < (vmin + 0.375 * dv)) {
          c.r = vmin;
          c.g = mx - 0.5*vmax;
          c.b = vmax;
     } else if (v < (vmin + 0.625 * dv)) {
          c.r = mx - 1.5*vmax;
          c.g = vmax;
          c.b = -mx + (vmax - 1.5*(vmin-vmax));
     } else if (v < (vmin + 0.875 * dv)) {
          c.r = vmax;
          c.g = -mx + (vmax - 2.5*(vmin-vmax));
          c.b = vmin;
     } else {
          c.r = -mx + 4.5*vmax;
          c.g = vmin;
          c.b = vmin;
     }     
     return(c);
}

double GetGray_matlab(Color_t c, double vmin, double vmax)
{
     double v = 0;
     //double slope = 0.5*vmax / (0.125*vmax - vmin);
     //double slope = (vmax - 0.5*(vmax-vmin)) / (0.125*(vmax-vmin)-vmin);
     //double slope = 0.5*(vmax-vmin) / (0.125*(vmax-vmin)-vmin);
     //double slope = 0.5*(vmax-vmin) / (0.125*vmax - 1.125*vmin);
     double slope = 4;
     
     if (c.g == vmin && c.r == vmin) {     
          //v = (c.b - 0.5*vmax) / slope;
          v = (c.b - 0.5*(vmax-vmin)) / slope;
     } else if (c.b == vmax && c.r == vmin) {
          //v = (c.g + 0.5*vmax) / slope;          
          v = (c.g + 0.5*(vmax-vmin) - vmin) / slope;
     } else if (c.g == vmax) {
          v = (c.r + 1.5*(vmax-vmin) - vmin) / slope;
          //v = (c.r - vmax + 5.0/2.0*(vmax-vmin)) / slope;
     } else if (c.b == vmin && c.r == vmax) {
          //v = (c.g - ((vmax - 2.5*(vmin-vmax)))) / (-slope);
          v = (c.g - 3.5*(vmax-vmin) - vmin) / -slope;
     } else if (c.g == vmin && c.b == vmin) {
          //v = (c.r - 4.5*vmax) / (-slope);
          v = (c.r - 4.5*(vmax) + 0.5*vmin) / -slope;
     }
     return v;
}

Color_t GetColor(double v, double vmin, double vmax)
{
     Color_t c = {255.0,255.0,255.0}; // white
     //Color_t c = {1.0,1.0,1.0}; // white
     double dv;

     if (v < vmin) {
          v = vmin;
     } else if (v > vmax) {
          v = vmax;
     }
     
     dv = vmax - vmin;

     if (v < (vmin + 0.25 * dv)) {
          c.r = vmin;
          c.g = 4.0*v*(vmax-vmin)/vmax;
          c.b = vmax;
     } else if (v < (vmin + 0.5 * dv)) {
          c.r = vmin;
          c.g = vmax;          
          c.b = 4.0*v*(vmin-vmax)/vmax-2*(vmin-vmax);
     } else if (v < (vmin + 0.75 * dv)) {
          c.r = 4.0*v*(vmax-vmin)/vmax-2*(vmax-vmin);
          c.g = vmax;
          c.b = vmin;          
     } else {
          c.r = vmax;
          c.g = 4.0*v*(vmin-vmax)/vmax-4.0*(vmin-vmax);
          c.b = vmin;
     }

     return(c);
}


int GetGray(Color_t c, double vmin, double vmax)
{
     int v = 0;
     
     if (c.r == vmin && c.b == vmax) {
          v = c.g * vmax / (4.0*(vmax-vmin));
     } else if (c.r == vmin && c.g == vmax) {
          v = (c.b + 2*(vmin-vmax)) / (4.0*(vmin-vmax)/vmax);
     } else if (c.g == vmax && c.b == vmin) {
          v = (c.r + 2*(vmax-vmin)) / (4.0*(vmax-vmin)/vmax);
     } else {
          v = (c.g + 4.0*(vmin-vmax)) / (4.0*(vmin-vmax)/vmax);
     }
     return v;
}

}
